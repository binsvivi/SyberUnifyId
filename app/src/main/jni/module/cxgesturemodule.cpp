#include "cxgesturemodule.h"
#include "cxgesture.h"

void trainGestureModule() {
//    const char *path = "/home/xizhi/UnifyID/module/slide/xizhi/slideModule";
    const char *path = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/module/gesture/shabin/gestureModule";

//    const char *featPath = "/home/xizhi/UnifyID/feature/slide/train/xizhi.feaures";
    const char *featPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/feature/gesture/train/shabin.feaures";


    CxGestureModule module;
    module.loadFeats( featPath );
    module.factored( GESTURE_FEATURE_DIM_SIZE );
    module.dynaCluster( 2, 1 );
    module.train();
//    module.saveModule(path);
    module.save(path);

    module.clear();
}

void testGestureModule() {
 //   const char *path = "/home/xizhi/UnifyID/module/slide/xizhi/slideModule";
    const char *path = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/module/gesture/shabin/gestureModule";

//    const char *featPath = "/home/xizhi/UnifyID/feature/slide/test/xizhi.feaures";
  //  const char *featPath = "/home/xizhi/UnifyID/feature/slide/test/shabin.feaures";
    const char *featPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/feature/gesture/test/xizhi.feaures";

    CxGestureModule module;
    module.load( path );

    CxGestureModule tx;
    tx.loadFeats( featPath );
//    module.loadModule( path );

    double p = 0.;
    int n = tx.nodeSize();
    printf("%d ==== \n",n);
    char** fpath = (char**)malloc( sizeof(char*) * n );
    FILE *fp = fopen( featPath, "r" );
    if ( fp ) {
        char buffer[1024] = {0};
        int len = 0;
        int i = 0;
        while( fscanf(fp, "%s", buffer) != EOF ) {
            len = strlen(buffer);
            char *tmp = (char*)malloc(len+1);
            memcpy( tmp, buffer, len );
            tmp[len] = 0;
            fpath[i++] = tmp;
            memset( buffer, 0, 1024 );
        }
    }
    int label = 0;
    int tnum = 0;
    int fnum = 0;
    for ( int i = 0; i < n; i++ ) {
        label = 0;
//        p = svm_predict( module.module(), module.nodes()[i] );
        p = module.test( tx.nodes()[i] ,label);
        if ( p < 0 )
            fnum++;
        else
            tnum++;
        fprintf( stderr, "%s :  [%d] model probability %lf \n", fpath[i], label, p );
//        fprintf( stderr, "%s :  %lf \n", fpath[i], p );
    //    printf("%s :  %lf \n", fpath[i], p);
        free( fpath[i] );
    }

    free( fpath );
    fprintf( stderr, "test %d ture feature, accept rate = %lf\n", n, (double)tnum / n );
    tx.clear();
}

int CXGestureNode::ref = 0;
int CXGestureNode::dataSize = sizeof(double) * CXGestureNode::sdm_dim;

CXGestureNode::CXGestureNode()
{}

CXGestureNode::~CXGestureNode()
{
    ref--;
    clear();
}

void CXGestureNode::clear()
{}

double* CXGestureNode::val()
{
    return m_data;
}

double CXGestureNode::distanceTo( CXDMNode *target )
{
    const double MIN_VAR = 1E-10;
    double dist = 0.;
    double* val = target->val();
    for ( int i = 0; i < sdm_dim; i++ ) {
        double tmp = std::max(MIN_VAR, (m_data[i] - val[i])*(m_data[i] - val[i]));
        //tmp *= tmp;
        //tmp+=tmp;
   //     fprintf(stderr, "index = [%d], value is [%f]\n", i, tmp);
        dist += tmp;
    }
    dist = sqrt( dist );
    //dist = 0.5*log(dist);
    return dist;
}

CXDMNode& CXGestureNode::operator =( CXDMNode &target )
{
    double* val = target.val();
    memcpy( m_data, val, dataSize );
    return *this;
}

CXDMNode& CXGestureNode::operator +=( CXDMNode &target )
{
    double* val = target.val();
    for ( int i = 0; i < sdm_dim; i++ ) {
        m_data[i] += val[i];
    }
    return *this;
}

CXDMNode& CXGestureNode::operator +=( CXDMNode *target )
{
    double* val = target->val();
    for ( int i = 0; i < sdm_dim; i++ ) {
        m_data[i] += val[i];
    }
    return *this;
}

CXDMNode& CXGestureNode::operator /=( double div )
{
    for ( int i = 0; i < sdm_dim; i++ ) {
        m_data[i] /= div;
    }
    return *this;
}

void CXGestureNode::zero()
{
    memset( m_data, 0, dataSize );
}

CXDMNode* CXGestureNode::copy()
{
    CXGestureNode *pNew = _newNodes();
    double* val = pNew->val();
    memcpy( val, m_data, dataSize );
    return pNew;
}

CXDMNode* CXGestureNode::newNodes( int num )
{
    return _newNodes( num );
}

CXDMNode** CXGestureNode::newNodePointers( int num )
{
    if ( num > 0 ) {
        CXDMNode **list = CXDFMallocPoints( CXDMNode, num );
        for ( int i = 0; i < num; i++ )
            list[i] = _newNodes();
        return list;
    }
    return NULL;
}

void CXGestureNode::load( svm_node *snode )
{
    //gesture area
    m_data[0] = snode[GESTURE_END_X].value; //position
    m_data[1] = snode[GESTURE_END_Y].value;
    m_data[2] = snode[GESTURE_END_Z].value;
    m_data[3] = snode[GESTURE_LDP_X].value;
    m_data[4] = snode[GESTURE_LDP_Y].value;
    m_data[5] = snode[GESTURE_LDP_Z].value;
    m_data[6] = snode[GESTURE_LDP_DIV].value; //div

    m_data[7] = snode[GESTURE_GLOBAL_DIV_VAR].value;//div var
    //gesture arc
    m_data[8] = snode[GESTURE_GLOBAL_DISTANCE].value;//slide front distance
    m_data[9] = snode[GESTURE_GLOBAL_LENGTH].value; //slide back distance
    m_data[10] = snode[GESTURE_FRONT_DISTANCE].value; //slide back distance
    m_data[11] = snode[GESTURE_FRONT_LENGTH].value; //slide back distance
    m_data[12] = snode[GESTURE_BACK_DISTANCE].value; //slide back distance
    //gesture velocity
    m_data[13] = snode[GESTURE_GLOBAL_VEL].value; //slide velocity
   //press
}

void CXGestureNode::load( double *featVals )
{
    //slide area
    m_data[0] = featVals[GESTURE_END_X]; //position
    m_data[1] = featVals[GESTURE_END_Y];
    m_data[2] = featVals[GESTURE_END_Z];
    m_data[3] = featVals[GESTURE_LDP_X];
    m_data[4] = featVals[GESTURE_LDP_Y]; //div
    m_data[5] = featVals[GESTURE_LDP_Z]; //div mean
    //slide arc
    m_data[6] = featVals[GESTURE_LDP_DIV];//slide front distance
    m_data[7] = featVals[GESTURE_GLOBAL_DIV_VAR];//slide front distance
    m_data[8] = featVals[GESTURE_GLOBAL_DISTANCE]; //slide back distance
    m_data[9] = featVals[GESTURE_GLOBAL_LENGTH]; //slide back distance
    //slide velocity
    m_data[10] = featVals[GESTURE_FRONT_DISTANCE]; //slide velocity
   //press
    m_data[11] = featVals[GESTURE_FRONT_LENGTH]; //slide pressure
    m_data[12] = featVals[GESTURE_BACK_DISTANCE]; //slide back distance
    m_data[13] = featVals[GESTURE_GLOBAL_VEL]; //slide velocity

}


void CXGestureNode::setVal( double *pVal )
{
    if ( pVal )
        memcpy( m_data, pVal, sizeof(double) * sdm_dim );
}


CxGestureModule::CxGestureModule()
{
  //  m_model = NULL;
  //  m_features = NULL;
  //  m_featTags = NULL;
  //  m_model = NULL;
  //  m_numFeatures = 0;

    //    m_model = NULL;
        m_features = NULL;
    //    m_featTags = NULL;
        m_numFeatures = 0;
        m_nodes = NULL;

        m_modules = NULL;
        m_nModule = 0;
        m_probs = NULL;
        m_meanVals = NULL;
        m_bFactor = false;

        m_factors = NULL;
        m_featDimSize = 0;


}

CxGestureModule::~CxGestureModule()
{
    clear();
}

void CxGestureModule::clear()
{
    releaseFeatures();
    releaseModule();

    CXDFFreeDouble( m_factors );
    CXDFFreeDouble( m_meanVals );
    CXDFFreeDouble( m_factors );
    m_bFactor = false;
    m_featDimSize = 0;

    m_kmeans.clear();
}

int CxGestureModule::load( const char *filePath )
{
    clear();
    FILE *fp = fopen( filePath, "r" );
    if ( !fp )
        return -1;

    int nval = 0;
    double *dVal = NULL;


    fscanf( fp, "featDimSize = %d\n", &m_featDimSize );
    fscanf( fp, "model number  = %d\n", &m_nModule );
    fscanf( fp, "factors = %d\n", &nval );
    if ( nval )
        m_bFactor = true;
    else
        m_bFactor = false;

    if ( m_bFactor ) {
        m_meanVals = CXDFMallocDouble( m_featDimSize );
        dVal = m_meanVals;
        for ( int i = 0; i < m_featDimSize; i++, dVal++ )
            fscanf( fp, "%lf", dVal);

        m_factors = CXDFMallocDouble( m_featDimSize );
        double *factor = m_factors;
        for ( int i = 0; i < m_featDimSize; i++, factor++ )
            fscanf( fp, "%lf", factor);
    }

    fscanf( fp, "%d", &nval );
    if ( nval ) {
        m_probs = CXDFMallocDouble( m_nModule );
        dVal = m_probs;
        for ( int i = 0; i < m_nModule; i++, dVal++ )
            fscanf( fp, "%lf", dVal );
    }

    char str[7] = {0};
    memset( str, 0, 7 );
    fscanf( fp, "%s", str );

    if ( !strcmp(str, "kmeans") ) {
    readKMeans( m_kmeans, fp );
    m_kmeans.means = CXDFMallocPoints( CXDMNode, m_kmeans.nCluster );
    double *pVal = m_kmeans.meanVals;
    for ( int i = 0; i < m_kmeans.nCluster; i++ ) {
        CXGestureNode *pNode  = CXGestureNode::_newNodes();
        pNode->setVal( pVal );
        pVal += m_kmeans.nDimSize;
        m_kmeans.means[i] = pNode;
    }
    memset( str, 0, 7 );
    fscanf( fp, "%s", str );
    }

    if ( !strcmp(str, "svm" ) ) {
    m_modules = CXDFMallocPoints( svm_model, m_nModule );
    char buffer[1024] = {0};
    for ( int k = 0; k < m_kmeans.nCluster; k++ ) {
        memset( buffer, 0, 1024 );
        fscanf( fp, "%s", buffer );
        svm_model *pModel = svm_load_model( buffer );
        m_modules[k] = pModel;
    }
    }

    fclose( fp );
    return 0;
}


int CxGestureModule::loadModule( const char *filePath )
{
    m_model = svm_load_model( filePath );
    return 0;
}

int CxGestureModule::loadFeats( const char *filePath )
{
    /*
    clear();
    m_featDimSize = GESTURE_FEATURE_DIM_SIZE;

    FILE *fp = fopen( filePath, "r" );
    if ( !fp )
        return -1;
    FILE *featFile = NULL;
    char pathBuffer[1024] = { 0 };
    int len = 0;
    const char *featExt = ".feat";
    int extSize = strlen( featExt );
    char *ptr = NULL;
    int num = 0;
    while( fscanf( fp, "%s", pathBuffer ) != EOF ) {
        len = strlen( pathBuffer );
        ptr = pathBuffer + len - extSize;
        if ( !strncmp( ptr, featExt, extSize) )
            num++;
    }
    if ( !num )
        return 0;

    fseek( fp, 0, SEEK_SET );
    releaseFeatures();
   // int incr = gestureFeatureSize + 1;
    int incr = GESTURE_FEATURE_DIM_SIZE + 1;

    m_numFeatures = num;
    len = sizeof(svm_node*) * m_numFeatures;
    m_nodes = ( svm_node**) malloc( len );
    memset( m_nodes, 0, len );
    len = sizeof(svm_node)* m_numFeatures * incr;
    m_features = ( svm_node*) malloc( len );
    memset( m_features, 0, len );
    len = sizeof(double) * m_numFeatures;
    m_featTags = (double*) malloc(len);
    memset( m_featTags, 0, len );

    num = 0;
    svm_node *pNode = m_features;
    while( fscanf( fp, "%s", pathBuffer ) != EOF ) {
        featFile = fopen( pathBuffer, "r" );
        if ( !featFile )
            continue;
        m_nodes[num] = pNode;
        m_featTags[num] = 1;
       // readFeature( featFile, pNode, gestureFeatureSize );
        readFeature( featFile, pNode, GESTURE_FEATURE_DIM_SIZE );
        num++;
        pNode += incr;
        memset( pathBuffer, 0, 1024);
    }

    return 0;
*/
    clear();

    m_featDimSize = GESTURE_FEATURE_DIM_SIZE;

    FILE *fp = fopen( filePath, "r" );
    if ( !fp )
        return -1;
    FILE *featFile = NULL;
    char pathBuffer[1024] = { 0 };
    int len = 0;
    const char *featExt = ".feat";
    int extSize = strlen( featExt );
    char *ptr = NULL;
    int num = 0;
    while( fscanf( fp, "%s", pathBuffer ) != EOF ) {
        len = strlen( pathBuffer );
        ptr = pathBuffer + len - extSize;
        if ( !strncmp( ptr, featExt, extSize) )
            num++;
    }
    if ( !num )
        return 0;

    fseek( fp, 0, SEEK_SET );
//    releaseFeatures();
    int incr = GESTURE_FEATURE_DIM_SIZE + 1;
    m_numFeatures = num;
    //sample x
    len = sizeof(svm_node*) * m_numFeatures;
//    m_nodes = ( svm_node**) malloc( len );
    m_nodes = CXDFMallocPoints( svm_node, m_numFeatures );
    memset( m_nodes, 0, len );
    //feature val
    len = sizeof(svm_node)* m_numFeatures * incr;
    m_features = ( svm_node*) malloc( len );
    memset( m_features, 0, len );

//    len = sizeof(double) * m_numFeatures;
//    m_featTags = (double*) malloc(len);
//    memset( m_featTags, 0, len );

    num = 0;
    svm_node *pNode = m_features;
    while( fscanf( fp, "%s", pathBuffer ) != EOF ) {
        featFile = fopen( pathBuffer, "r" );
        if ( !featFile )
            continue;
        m_nodes[num] = pNode;
//        m_featTags[num] = 1;
        readFeature( featFile, pNode, GESTURE_FEATURE_DIM_SIZE );
        num++;
        pNode += incr;
        memset( pathBuffer, 0, 1024);
    }

    return 0;

}

void CxGestureModule::dynaCluster( int nCluster, int offset )
{
    if ( m_numFeatures < 1 || !m_features || ! m_nodes )
        return;

    CXGestureNode **datas = CXDFMallocPoints( CXGestureNode, m_numFeatures );
    CXGestureNode *nodes = CXGestureNode::_newNodes( m_numFeatures );
    CXGestureNode *ptr = nodes;
    svm_node **tmp = m_nodes;

    for ( int i = 0; i < m_numFeatures; i++, ptr++, tmp++ ) {
        datas[i] = ptr;
        if ( *tmp )
            ptr->load( (*tmp) );
    }

    CXDmeans dms;
    int nK = nCluster;
//    int offset = 5;
    fprintf( stderr, "init k = %d, offset = %d \n", nK, offset );
    dms.dynamicCluster( (CXDMNode**)datas, m_numFeatures, nK, offset, &m_kmeans );
    m_kmeans.nDimSize = CXGestureNode::sdm_dim;

    CXDFFreePointers( datas );
    delete[] nodes;
}

void CxGestureModule::factored( int dimSize )
{
    if ( m_featDimSize != dimSize ) {
        m_featDimSize = dimSize;
        CXDFFreeDouble( m_factors);
        CXDFFreeDouble( m_meanVals );
    }

    if ( !m_meanVals )
        m_meanVals = CXDFMallocDouble( m_featDimSize );
    if ( !m_factors )
        m_factors = CXDFMallocDouble( m_featDimSize );


    if ( !m_features || m_numFeatures < 1 )
        return;

    svm_node *feat = m_features;
    svm_node *limit = m_features + m_numFeatures * ( m_featDimSize + 1);
    double *minBuffer = CXDFMallocDouble( m_featDimSize );
    double *maxBuffer = m_factors;
    int size = sizeof( double ) * m_featDimSize;
    memset( minBuffer, 0, size );
    memset( maxBuffer, 0, size );
    double *min = minBuffer - 1;
    double *max = maxBuffer - 1;
    double *means = m_meanVals - 1;
    for ( int i = 0; i < m_featDimSize; i++, feat++ ) {
        min[feat->index] = feat->value;
        max[feat->index] = feat->value;
        means[feat->index] = feat->value;
    }

    while ( feat < limit ) {
        if ( feat->index < 0 ) {
            feat++;
            continue;
        }
        means[feat->index] += feat->value;
        if ( min[feat->index] > feat->value )
            min[feat->index] = feat->value;
        if ( max[feat->index] < feat->value )
            max[feat->index] = feat->value;
        feat++;
    }

    for ( int i = 0; i < m_featDimSize; i++ ) {
        m_meanVals[i] /= (double)m_numFeatures;
        fprintf( stderr, "feat[%2d] : min %lf ; max %lf ; span %lf; scale : %lf; mean: %lf    ",
                 i, minBuffer[i], maxBuffer[i], maxBuffer[i] - minBuffer[i], gesture_feature_scale[i], m_meanVals[i] );
        m_factors[i] = maxBuffer[i] - minBuffer[i];
        m_factors[i] /= gesture_feature_scale[i];
    }

    feat = m_features;
    memset( minBuffer, 0, size );
  //  double *H = m_factors - 1;
    while ( feat < limit ) {
        if ( feat->index < 0 ) {
            feat++;
            continue;
        }
        double delta = feat->value - means[feat->index];
        delta *= delta;
        min[feat->index] += delta;
     //   feat->value /= H[feat->index];
        feat++;
    }

    //ceshi
    fprintf( stderr, "//////////////////// var ////////////////////\n");
    double dist = 0.;
    for ( int i = 0; i < m_featDimSize; i++ ) {
        minBuffer[i] /=  (double)m_numFeatures;
        fprintf( stderr, "var[%d] = %lf\n", i, minBuffer[i] );
        dist += minBuffer[i];
    }
    fprintf( stderr, "global dist = %lf\n", dist );

    feat = m_features;
    double *H = m_factors - 1;
    while ( feat < limit ) {
        if ( feat->index < 0 ) {
            feat++;
            continue;
        }

        feat->value -= means[feat->index];
        feat->value /= H[feat->index];
        feat++;
    }

    m_bFactor = true;

    CXDFFreeDouble( minBuffer );
}

double CxGestureModule::test( svm_node *x , int &label)
{
    if ( m_factors && m_factors && m_meanVals) {
        //ceshi
        double dist = 0.;

        for ( int i = 0; i < m_featDimSize; i++ )  {
            x[i].value -= m_meanVals[i];
            //ceshi
            {
            double tmp = x[i].value;
            tmp *= tmp;
            dist += tmp;
            }
          //  x[i].index = i + 1;
            x[i].value /= m_factors[i];
        }
        fprintf( stderr, "global dist offset :  %lf\n", dist );

    }
    double ret =  testStdNode( x, label );
    return ret;
    /*
    int label = 0;
    if ( m_nModule > 1 ) {
        CXGestureNode *pNode = new CXGestureNode;
        pNode->load( x );
        CXDmeans::getLabel( pNode, &m_kmeans, m_kmeans.nCluster, label );
        delete pNode;
    }

    svm_model *pModel = m_modules[label];
    double ret = svm_predict( pModel, x );
    return ret;
    */
}

double CxGestureModule::testStdNode( svm_node *x, int &label )
{
//    int label = 0;
    label = 0;
    if ( m_nModule > 1 ) {
        CXGestureNode *pNode = CXGestureNode::_newNodes();
        pNode->load( x );
        CXDmeans::getLabel( pNode, &m_kmeans, m_kmeans.nCluster, label );
        delete pNode;
    }

    svm_model *pModel = m_modules[label];
    double ret = svm_predict( pModel, x );
    return ret;
}

void CxGestureModule::train()
{
    if ( m_modules )
        releaseModule();

    if ( m_kmeans.nCluster == 1 ) {
        return trainOne();
    }
    m_nModule = m_kmeans.nCluster;
    //malloc model pointer list
    m_modules = CXDFMallocPoints(svm_model, m_kmeans.nCluster );
    if  ( m_probs ) {
        CXDFFreeDouble( m_probs );
    }
    m_probs = CXDFMallocDouble( m_nModule );
    //get max unmber of feat class
    int max = m_kmeans.counts[0];
    m_probs[0] = (double)m_kmeans.counts[0] / m_numFeatures;
    for ( int k = 1; k < m_kmeans.nCluster; k++ ) {
        if ( max < m_kmeans.counts[k] )
            max = m_kmeans.counts[k];
        m_probs[k] = (double)m_kmeans.counts[k] / m_numFeatures;
    }

    svm_problem prob;
    svm_parameter param;
    svm_node *snode = NULL;
    svm_node *limit = m_features + m_numFeatures;
    int *label = NULL;

    double *y = CXDFMallocDouble( max );
    svm_node **x = CXDFMallocPoints( svm_node, max );

    //prob
    prob.x = x;
    prob.y = y;

    //param
    param.svm_type = ONE_CLASS; //类型
//        param.kernel_type = RBF; //核函数
//        param.kernel_type = POLY; //核函数
    param.kernel_type = SIGMOID; //核函数
    param.degree = 5; //多项式参数
    param.gamma = 1.0f; //核函数参数
    param.coef0 = 0.5f;
    param.nu = 0.5;
    param.cache_size = 100; //100mb buffer
    param.C = 1;
    param.eps = 1e-8;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    for ( int i = 0; i < max; i++ )
        y[i] = 1.;
    int incr = m_featDimSize + 1;

    for ( int k = 0; k < m_kmeans.nCluster; k++ ) {
        prob.l = m_kmeans.counts[k];
      //  snode = m_features;
        label = m_kmeans.labels;
        int num = 0;
        /*
        while ( snode < limit ) {
            if ( *label == k ) {
                x[num] = snode;
                num++;
                if ( num == prob.l )
                    break;
            }
            label++;
            snode++;
        }//endwhile
        */
        for ( int i = 0; i < m_numFeatures; i++, label++ ) { //search all features
            if ( *label == k ) { //when feature label = current train cluster index
                x[num] = m_features + i * incr; //get start pointer
                num++;
                if ( num == prob.l )
                    break;
            }
        }
        fprintf( stderr, "train %d svm model, count = %d ...\n", k, prob.l );

        svm_model *pModel = trainModel( prob, param );
        m_modules[k] = pModel;
    }

   // m_nModule = m_kmeans.nCluster;

    CXDFFreeDouble( y );
    CXDFFreePointers( x );

/*    if ( !m_model ) {
        svm_problem prob;
        svm_parameter param;
        prob.l = m_numFeatures;
        prob.x = m_nodes;
        prob.y = m_featTags;

        param.svm_type = ONE_CLASS; //类型
//        param.kernel_type = RBF; //核函数
//        param.kernel_type = POLY; //核函数
        param.kernel_type = SIGMOID; //核函数
        param.degree = 5; //多项式参数
        param.gamma = 1.0f; //核函数参数
        param.coef0 = 0.5f;
        param.nu = 0.5;
        param.cache_size = 100; //100mb buffer
        param.C = 1;
        param.eps = 1e-8;
        param.p = 0.1;
        param.shrinking = 1;
        param.probability = 0;
        param.nr_weight = 0;
        param.weight_label = NULL;
        param.weight = NULL;

        const char* message  = svm_check_parameter( &prob, &param );
        if ( message )
            fprintf( stderr, "%s", message );
        m_model = svm_train( &prob, &param );
    }*/
}

svm_model* CxGestureModule::trainModel( svm_problem &prob, svm_parameter &param )
{
    const char* message  = svm_check_parameter( &prob, &param );
    if ( message )
        fprintf( stderr, "%s", message );
    svm_model *pModel = svm_train( &prob, &param );
    return pModel;
}

void CxGestureModule::trainOne()
{
    m_modules = CXDFMallocPoints(svm_model, m_kmeans.nCluster );
    svm_problem prob;
    svm_parameter param;
    double *featTags = CXDFMallocDouble(m_numFeatures);
    for ( int i = 0; i < m_numFeatures; i++ )
        featTags[i] = 1.;
    prob.l = m_numFeatures;
    prob.x = m_nodes;
    prob.y = featTags;

    param.svm_type = ONE_CLASS; //类型
        param.kernel_type = RBF; //核函数
//        param.kernel_type = POLY; //核函数
//    param.kernel_type = SIGMOID; //核函数
    //param.degree = 5; //多项式参数
    param.degree = 3; //多项式参数
  //  param.gamma = 1.0f; //核函数参数
    param.gamma = 0.02f; //核函数参数
    //param.coef0 = 0.5f;
    param.coef0 = 0.0f;
   // param.nu = 0.5;
    param.nu = 0.01;
    param.cache_size = 100; //100mb buffer
    param.C = 1;
    //param.eps = 1e-8;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;

    const char* message  = svm_check_parameter( &prob, &param );
    if ( message )
        fprintf( stderr, "%s", message );
    svm_model *pModel = svm_train( &prob, &param );
    m_modules[0] = pModel;

    m_nModule = 1;
    return;
}

void CxGestureModule::save( const char *filePath )
{
    /*
    FILE *fp = fopen( filePath, "w" );
    if ( !fp )
        return;
    //model file path str
    char model_path[1024] = {0};
    memset( model_path, 0, 1024 );
    //ext loc
    int span = strlen( filePath );
    char *ptr = model_path + span;
    int size = 1024 - span;
    //ext name str
    char ext[12] = { 0 };
    memcpy( model_path, filePath, span );
    svm_model **model = m_modules;
    fprintf( fp, "featDimSize = %d\n", m_featDimSize ); //factors
    if ( m_bFactor ) {
        fprintf( fp, "factors = 1\n");
        for ( int i = 0; i < m_featDimSize; i++ )
            fprintf( fp, "%lf\n", m_factors[i] );
    } else
        fprintf( fp, "factors = 0\n");

    //print k cluster number
    fprintf( fp, "nCluster = %d\n", m_nModule );
    for ( int k = 0; k < m_nModule; k++ ) {
        //reset root path, and  ext str name
        memset( ptr, 0, size );
        memset( ext, 0, 12 );
        //set ext name
        sprintf( ext, "-model%d", k );
        memcpy( ptr, ext, strlen(ext) );
        //save svm model
        svm_save_model( model_path, *model );
        //print svm model file path
        fprintf( fp, "%s\n", model_path );
        model++;
    }

    fclose( fp );
*/


    FILE *fp = fopen( filePath, "w" );
    if ( !fp )
        return;
    //model file path str
    char model_path[1024] = {0};
    memset( model_path, 0, 1024 );
    //ext loc
    int span = strlen( filePath );
    memcpy( model_path, filePath, span );
    char *ptr = model_path + span;
    int size = 1024 - span;
    //ext name str
    char ext[12] = { 0 };
    svm_model **model = m_modules;
    fprintf( fp, "featDimSize = %d\n", m_featDimSize ); //factors
    fprintf( fp, "model number  = %d\n", m_nModule );
    if ( m_bFactor ) {
        fprintf( fp, "factors = 1\n");
        for ( int i = 0; i < m_featDimSize; i++ )
            fprintf( fp, "%lf\n", m_meanVals[i] );
        for ( int i = 0; i < m_featDimSize; i++ )
            fprintf( fp, "%lf\n", m_factors[i] );
    } else
        fprintf( fp, "factors = 0\n");

    if ( m_probs ) {
        fprintf( fp, "%d\n", 1 );
        for ( int i = 0; i < m_nModule; i++ )
            fprintf( fp, "%lf\n", m_probs[i] );
    } else
        fprintf( fp, "%d\n", 0 );


    fprintf( fp, "%s\n", "kmeans" );
    printKMeans( m_kmeans, fp );

    fprintf( fp, "%s\n", "svm" );
    //print k cluster number
    for ( int k = 0; k < m_nModule; k++ ) {
        //reset root path, and  ext str name
        memset( ptr, 0, size );
        memset( ext, 0, 12 );
        //set ext name
        sprintf( ext, "-model%d", k );
        memcpy( ptr, ext, strlen(ext) );
        //save svm model
        svm_save_model( model_path, *model );
        //print svm model file path
        fprintf( fp, "%s\n", model_path );
        model++;
    }

    fclose( fp );

}


void CxGestureModule::saveModule( const char *filePath )
{
    svm_save_model( filePath, m_model );
}


void CxGestureModule::releaseFeatures()
{
  //  if ( m_features ) {
  //      CXDFFREE( m_features );
  //      CXDFFREE( m_nodes );
  //  }
  //  CXDFFREE( m_featTags );

    if ( m_features ) {
        CXDFFREE( m_features );
        CXDFFreePointers( m_nodes );
    }
//    CXDFFREE( m_featTags );
    m_numFeatures = 0;
}

void CxGestureModule::releaseModule()
{
 //   if ( m_model ) {
 //       svm_free_and_destroy_model(&m_model);
  //  }

    if ( m_modules ) {
        svm_model **pModel = m_modules;
        for ( int i = 0; i < m_nModule; i++, pModel++ ) {
            if ( m_modules[i] )
                svm_free_and_destroy_model( pModel );
        }

        CXDFFreePointers( m_modules );
    }

    m_nModule = 0;
}

void CxGestureModule::readFeature( FILE *fp, svm_node *node, int size )
{
    int i = 0;
    int j = 1;
    float fval = 0.;
    while( i < size && fscanf( fp, "%f", &fval ) != EOF ) {
        node[i].index = j;
        node[i].value = fval;
       // fprintf( stderr, "node value: %f \n", fval );
        fval = 0.f;
        i = j;
        j++;
    }
    node[i].index = -1;
}
/*
int CxGestureModule::load( const char *filePath )
{
    clear();
    FILE *fp = fopen( filePath, "r" );
    if ( !fp )
        return -1;

    int nval = 0;
    fscanf( fp, "featDimSize = %d\n", &m_featDimSize );
    fscanf( fp, "factors = %d\n", &nval );
    if ( nval )
        m_bFactor = true;
    else
        m_bFactor = false;

    if ( m_bFactor ) {
        m_factors = CXDFMallocDouble( m_featDimSize );
        double *factor = m_factors;
        for ( int i = 0; i < m_featDimSize; i++, factor++ )
            fscanf( fp, "%lf", factor);
    }

    readKMeans( m_kmeans, fp );
    m_kmeans.means = CXDFMallocPoints( CXDMNode, m_kmeans.nCluster );
    double *pVal = m_kmeans.meanVals;
    for ( int i = 0; i < m_kmeans.nCluster; i++ ) {
        CXGestureNode *pNode  = new CXGestureNode;
        pNode->setVal( pVal );
        pVal += m_kmeans.nDimSize;
        m_kmeans.means[i] = pNode;
    }

    m_modules = CXDFMallocPoints( svm_model, m_nModule );
    char buffer[1024] = {0};
    for ( int k = 0; k < m_kmeans.nCluster; k++ ) {
        memset( buffer, 0, 1024 );
        fscanf( fp, "%s", buffer );
        svm_model *pModel = svm_load_model( buffer );
        m_modules[k] = pModel;
    }

    fclose( fp );
    return 0;
}
*/
