#include "cxdmeans.h"
//#include "ceshi.h"

void readKMeans( cxskmeans &kmeans, FILE *fp ) {
    int k = 0;
    int nDim = 0;
 //   fscanf( fp, "cluster = %d\n", &k );
 //   fscanf( fp, "dim = %d\n", &nDim );
    fscanf( fp, "%d", &k );
    fscanf( fp, "%d", &nDim );
    kmeans.initKMeans( k, nDim );
    k *= nDim;
    double *dval = kmeans.meanVals;
    for ( int i = 0; i < k; i++, dval++ ) {
        fscanf( fp, "%lf", dval );
    }
}

void printKMeans( cxskmeans &kmeans, FILE *fp ) {
    //fprintf( fp, "cluster = %d\n", kmeans.nCluster );
    fprintf( fp, "%d\n", kmeans.nCluster );
    int dimSize = kmeans.means[0]->dimSize();
//    fprintf( fp, "dim = %d\n", dimSize );
    fprintf( fp, "%d\n", dimSize );
    for ( int k = 0; k < kmeans.nCluster; k++ ) {
        double *val = kmeans.means[k]->val();
        for ( int i = 0; i < dimSize; i++ ) {
            fprintf( fp, "%lf\n", val[i] );
        }
    }
}

void loadKMeans( cxskmeans &kmeans, const char *filePath ) {
    FILE *fp = fopen( filePath, "r" );
    if ( !fp )
        return;
    readKMeans( kmeans, fp );
    fclose( fp );
}

void saveKMeans( cxskmeans &kmeans, const char *filePath ) {
    FILE *fp = fopen( filePath, "w" );
    if ( !fp )
        return;
    printKMeans( kmeans, fp );
    fclose( fp );
}

void freeDynaNode( CXDMNode **nodes, int num )
{
    if ( !nodes || num < 1 )
        return;
    for ( int i = 0; i < num; i++ ) {
        if ( nodes[i] ) {
            nodes[i]->clear();
            delete nodes[i];
        }
    }
//    free( nodes );
    CXDFFreePointers( nodes );
}

CXDmeans::CXDmeans()
{
    m_KMean = NULL;
    m_datas = NULL;
    m_num = 0;
    m_gMean = NULL;

    m_maxIterNum = 10000;
    m_endError = 0.000001;
}

CXDmeans::~CXDmeans()
{
    clear();
}

void CXDmeans::clear()
{
    CXDFDELETE( m_KMean );

   m_datas = NULL;
   m_num = 0;

   if ( m_gMean )
       delete m_gMean;

   m_maxIterNum = 10000;
   m_endError = 0.000001;
}

void CXDmeans::dynamicCluster( CXDMNode **data, int num, int k, int offset, cxskmeans *dst )
{
    return dynamicCluster( data, num, k, offset, m_maxIterNum, m_endError, dst );
}

void CXDmeans::dynamicCluster( CXDMNode **data, int num, int k, int offset,
                                                                                int maxIterNum, double endError, cxskmeans *dst )
{
    int i = k;
    int j = k;
    int minK = k - offset;
    if ( minK < 0 )
        minK = 1;
    int maxK = k + offset;
    int size =  maxK - minK+1;

    if ( !dst ) {
        if ( !m_KMean ) {
            m_KMean = new cxskmeans;
            dst = m_KMean;
        } else
            dst = m_KMean;
    }

    cxskmeans *kms = new cxskmeans[size];
    CXDMNode **next_means = (*data)->newNodePointers( maxK );
    int *counts = CXDFMallocInt( maxK );
    computeMean( data, num );
    cxskmeans *target = kms;
    target->nCluster = k;
    optimumInit( data, num, target, counts, next_means );
    cluster( target, data, num, maxIterNum, endError, counts, next_means );
    computeGlobalCost( target );
    cxskmeans* cur = kms + 1;
    cxskmeans *left = NULL;
    cxskmeans *right = NULL;
    do {
        bool bleft = false;
        if ( i > minK ) {
            i--;
            left = cur++;
            left->nCluster = i;
            optimumInit( data, num, left, counts, next_means );
            cluster( left, data, num, maxIterNum, endError, counts, next_means );
            computeGlobalCost( left );
            bleft = true;
        }
        bool bright = false;
        if ( j < maxK ) {
            j++;
            right = cur++;
            right->nCluster = j;
            optimumInit( data, num, right, counts, next_means );
            cluster( right, data, num, maxIterNum, endError, counts, next_means );
            computeGlobalCost( right );
            bright = true;
        }
        bool bf = false;
        if ( bleft && target->dist > left->dist )  {
            target = left;
            bf = true;
        }
        if ( bright && target->dist > right->dist ) {
            target = right;
            bf = true;
        }

        if ( !bf )
            break;
    } while( i > minK && j < maxK );


    dst->copy( target );
    dst->initCounts();
    cluster( dst, data, num,maxIterNum, endError, dst->counts, next_means );
//    cluster( &dst, data, num,maxIterNum, endError, counts, next_means );
    delete[] kms;
    freeDynaNode( next_means, maxK );
    CXDFFreeInt( counts );
}

void CXDmeans::init( CXDMNode **data, int num,  cxskmeans *km )
{
    if ( !km->means ) {
        fprintf( stderr, "not alloc means memory!");
        return;
    }
    srand( time(NULL) );
//    int size = num;
    int level = num *10;
    double *probabilitys = CXDFMallocDouble( num );
//    double *lp = probabilitys;
//    double *plimit = probabilitys + num;
//    double* x = (double*)malloc(sizeof(double) * m_dimNum );	// Sample data
//    int zerosize = sizeof(double)*m_dimNum;
    CXDMNode **ptr = data;
//    CXDMNode *limit = ptr + num;
    int label = 0;
    double dist = 0.;

    //init first mean
//    memset( m_kMean[0], 0, zerosize );
//    for ( int i = 0; i < size; i++ ) {
//        for ( int j = 0; j < m_dimNum; j++ ) {
//            m_kMean[0][j] += *ptr;
//            ptr++;
//        }
//    }
//    for ( int j = 0; j < m_dimNum; j++ ) {
//        m_kMean[0][j] /= size;
//    }

     km->zeroMeans();

//     while( ptr < limit ) {
//         km->means[0] += *ptr;
//         ptr++;
//     }
//     km->means[0] /= (double)num;

     if ( !m_gMean )
         computeMean( data, num );

     if ( !m_gMean ) {
         fprintf( stderr, "compute global mean error");
         return;
     }
     *km->means[0] = *m_gMean;

    int n = 1;
//    while( n < m_nCluster ) {
    while( n < km->nCluster ) {
        dist = 0.;
        ptr = data;
//        lp = probabilitys;
//        for ( int i = 0; i < size; i++ ) {
//            for ( int j = 0; j < m_dimNum; j++ ) {
//                x[j] = *ptr;
//                ptr++;
//            }
//            p[i] = getlable( x, n, &lable );
//            dist += p[i];
//        }
        for ( int i = 0; i < num; i++, ptr++ ) {
            probabilitys[i] = getLabel( *ptr, km, n, label );
            dist += probabilitys[i];
        }
//        for ( int i = 0; i < size; i++ ) {
//            p[i] /= dist;
//        }
//        double incr = 0;
//        for ( int i = 0; i < size; i++ ) {
//            p[i] += incr;
//            incr = p[i];
//        }
        double incr = 0.;
        for ( int i = 0; i < num; i++ ) {
            probabilitys[i] /= dist;
            probabilitys[i] += incr;
            incr = probabilitys[i];
        }

        double px = getRand( level );
        int pos = 0;
        while ( pos < num ) {
            if ( probabilitys[pos] > px )
                break;
            pos++;
        }

//        ptr = data + m_dimNum * pos;
//        for ( int j = 0; j < m_dimNum; j++ ) {
//            m_kMean[n][j] = *ptr;
//            ptr++;
//        }
        ptr = data + pos;
        *km->means[n] = **ptr;

        n++;
    }

    CXDFFreeDouble( probabilitys );
}

void CXDmeans::cluster( cxskmeans *km )
{
    return cluster( km, m_datas, m_num, m_maxIterNum, m_endError, NULL, NULL );
}

void CXDmeans::cluster( cxskmeans *km, int iterNum, double endError,
                                                        int *counts, CXDMNode **next_means )
{
    return cluster( km, m_datas, m_num, iterNum, endError, counts, next_means );
}

void CXDmeans::cluster(  cxskmeans *km, CXDMNode **data, int num,
                                                        int *counts, CXDMNode **next_means )
{
    return cluster( km, data, num, m_maxIterNum, m_endError, counts, next_means );
}

void CXDmeans::cluster(  cxskmeans *km, CXDMNode **data, int num, int maxIterNum, double endError,
                                                                                                                int *counts, CXDMNode **next_means )
{
//    fprintf( stderr, "void CXDmeans::cluster( float *data, int Num, int *lables )\n" );
    if ( km->nCluster >= num ) {
        fprintf( stderr, "data number must > k cluster!");
        return;
    }
//    int size = 0;
//    size=Num;
//    assert(size >= m_nCluster );

    // Recursion
    //用来存储输入的m_dimNum维特征向量
//    double* x = (double*) malloc( sizeof(double) * m_dimNum );	// Sample data
    int label = -1;		// Class index
    double iterNum = 0;
    double lastCost = 0;
    double currCost = 0;
    int unchanged = 0;
    bool loop = true;
//    int* counts = (int*) malloc( sizeof(int) * km->nCluster );
    bool bCounts = false;
    if ( !counts ) {
//        counts = (int*) malloc( sizeof(int) * km->nCluster );
        counts = CXDFMallocInt( km->nCluster);
        bCounts = true;
    }
    //存储新m_nCluster 个高斯模型对应的中心的种子,m_dimNum维特征向量
//    double** next_means = (double**)malloc( sizeof(double*) * m_nCluster );	// New model for reestimation
//    for (  int i = 0; i < m_nCluster; i++ ) {
//        next_means[i] = ( double*) malloc( sizeof(double) * m_dimNum );
//    }
//    CXDMNode **next_means = (*data)->newNodePointers( km->nCluster );
    bool bNexMeans = false;
    if ( !next_means ) {
        next_means = (*data)->newNodePointers( km->nCluster );
        bNexMeans = true;
    }
    CXDMNode **ptr = NULL;
    while ( loop ) {
        //reserve last cost
        lastCost = currCost;
        currCost = update( km, data, num, counts, next_means );
        // Terminal conditions
        iterNum++;
        if ( fabs( lastCost - currCost ) < endError * lastCost ) {
            unchanged++;
        } else
            unchanged= 0;

        if ( iterNum >= maxIterNum || unchanged >= 3 ) {
            loop = false;
        }

        //DEBUG
        //cout << "Iter: " << iterNum << ", Average Cost: " << currCost << endl;
    }

    // Output the lable file
//    m_distance = 0.;
//    for (  int i = 0; i < size; i++ ) {
//        for ( int j = 0; j < m_dimNum; j++ )
//                x[j] = data[i*m_dimNum+j];
//        m_distance += getlable(x, &lable);
//        lables[i] = lable;
//    }
    km->dist = 0.;
    ptr = data;
    if ( !km->labels ) {
        km->setLabels( num );
    }
    for ( int i = 0; i < num; i++, ptr++ ) {
        km->dist += getLabel( *ptr, km, km->nCluster, label );
        km->labels[i] = label;
    }

    if ( bCounts ) {
//        free( counts );
        CXDFFreeInt( counts );
    }
//    free( x );
//    for (  int i = 0; i < m_nCluster; i++ ) {
//        free( next_means[i] );
//    }
//    free( next_means );
    if ( bNexMeans )
        freeDynaNode( next_means, km->nCluster );
}

double CXDmeans::update( cxskmeans *km, CXDMNode **data, int num, int *counts, CXDMNode **next_means )
{
    double currCost = 0.;
    int label = -1;		// Class index

    //zero next Mean
    memset( counts, 0, sizeof(int) * km->nCluster );
//        for ( int i = 0; i < m_nCluster; i++ ) {
//            memset( next_means[i], 0, sizeof(double) * m_dimNum );
//        }
    for ( int i = 0; i < km->nCluster; i++ )
        next_means[i]->zero();
    //reserve last cost
//    lastCost = currCost;
    currCost = 0;
    CXDMNode **ptr = data;
    // Classification
    for ( int i = 0; i < num; i++, ptr++ ) {
        //get 输入向量
//            for ( int j = 0; j < m_dimNum; j++ )
//                x[j] = data[i*m_dimNum+j];
        //获得该向量最近的种子中心和距离
//            currCost += getlable( x, &lable );
        currCost += getLabel( *ptr, km, km->nCluster, label );
        //该种子聚类,总和++
        counts[label]++;
        //该种子中心向量累加
//            for ( int d = 0; d < m_dimNum; d++ ) {
//                next_means[lable][d] += x[d];
//            }
        *next_means[label] += **ptr;
    }
    //总的平均偏离距离
    currCost /= num;

    // Reestimation
    for (  int i = 0; i < km->nCluster; i++ ) {
        if ( counts[i] > 0 )  {
            //取均值作为新的中心
//                for ( int d = 0; d < m_dimNum; d++ ) {
//                    next_means[i][d] /= counts[i];
//                }
            *next_means[i] /= (double)counts[i];
            //重置
//                memcpy( m_kMean[i], next_means[i], sizeof(double) * m_dimNum );
            *km->means[i] = *next_means[i];
        }
    }

    return currCost;
}

void CXDmeans::optimumInit( CXDMNode **data, int num, cxskmeans *km, int *counts, CXDMNode **next_means )
{
    int limit = 10;
    int nIterNum = 20;
    double incr = 0.;
    double last = 0.;
    double endError = 0.001;
    bool bCounters = false;
    bool bNextMeans = false;
    if ( !counts ) {
//        counts = (int*) malloc( sizeof(int) *km->nCluster );
        counts = CXDFMallocInt( km->nCluster );
        bCounters = true;
    }
    if ( !next_means ) {
        next_means = (*data)->newNodePointers( km->nCluster );
        bNextMeans = true;
    }

    cxskmeans tmp;
    tmp.initWithData( data, km->nCluster );
    for ( int i = 0; i < limit; i++ ) {
        init( data, num, &tmp );
        tmp.dist = 0.;
        for ( int j = 0; j < nIterNum; j++ ) {
            last = tmp.dist;
            tmp.dist = update( &tmp, data, num, counts, next_means );
            if ( j ) {
                incr = fabs( tmp.dist - last );
                if ( incr < last *endError )
                    break;
            }
        }
        if ( !i )
            km->copy( &tmp );
        else if ( tmp.dist < km->dist )
            km->copy( &tmp );
    }

    if ( bCounters )
        CXDFFreeInt( counts );
    if ( bNextMeans )
        freeDynaNode( next_means, km->nCluster );

    tmp.clear();
}

double CXDmeans::getLabel( CXDMNode *x, cxskmeans *km, int k, int &label )
{
    double dist = -1;
    label = 0;
    CXDMNode **mean = km->means;
    for ( int i = 0; i < k; i++, mean++ ) {
        double temp = x->distanceTo( *mean );
        if ( temp < dist || dist < 0. ) {
            dist = temp;
            label = i;
        }
    }
    return dist;
}

double CXDmeans::getRand( int N )
{
    return (double) ( rand()%N ) / (double)N;
}

void CXDmeans::computeMean( CXDMNode **data, int num )
{
    if ( m_gMean ) {
        delete m_gMean;
        m_gMean = NULL;
    }
    m_gMean = (*data)->newNodes();
    for ( int i = 0; i < num; i++ )
        *m_gMean += data[i];

    *m_gMean /= (double)num;
}

void CXDmeans::computeGlobalCost( cxskmeans *km )
{
    if ( !m_gMean )
        return;

    double mdist = 0.;
    for ( int i = 0; i < km->nCluster; i++ ) {
        mdist += km->means[i]->distanceTo( m_gMean );
    }

    km->dist += mdist;
}
