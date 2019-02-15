#include "cxslidemodule.h"
//#include "cxslide.h"
#include <android/log.h>
#include <float.h>
//#define LOG_TAG "System.out.c"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOG_TAG "slideModule"

void trainSlideModule() {
//    const char *path = "/home/xizhi/UnifyID/module/slide/xizhi/slideModule";
//    const char *featPath = "/home/xizhi/UnifyID/feature/slide/train/xizhi.feaures";
//    const char *path = "/home/xizhi/ceshi/train3/model/slideModule";
 //   const char *featPath = "/home/xizhi/ceshi/train3/train.features";
    const char *path = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/slideModule";

    const char *featPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/train/xizhi.feaures";
    CxSlideModule module;
    module.loadFeats( featPath );
    module.factored( SLIDE_FEATURE_DIM_SIZE );
    memManager::print();
    CXSlideNode::print();
    module.dynaCluster( 1, 0 );
    module.train();
    module.save(path);
    module.clear();
}

void testSlideModule() {
//    const char *path = "/home/xizhi/UnifyID/module/slide/xizhi/slideModule";
//    const char *target = "/home/xizhi/ceshiModule";
//    const char *TruefeatPath = "/home/xizhi/UnifyID/feature/slide/test/xizhi.feaures";
////    const char *FalsefeatPath = "/home/xizhi/UnifyID/feature/slide/test/shabin.feaures";
//    const char *FalsefeatPath = "/home/xizhi/UnifyID/feature/slide/test/gao.feaures";

   // const char *path = "/home/xizhi/ceshi/train3/model/slideModule";
    const char *TruefeatPath = "/home/xizhi/ceshi/test3/test.features";
    const char *path = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/slideModule";

//    const char *featPath = "/home/xizhi/UnifyID/feature/slide/test/xizhi.feaures";
//    const char *featPath = "/home/xizhi/UnifyID/feature/slide/test/shabin.feaures";
    const char *featPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/test/shabin.feaures";


    //module
    CxSlideModule module;
    module.load( path );
//    module.save( target );

    //samples
    CxSlideModule tx;
    tx.loadFeats( featPath );
    //get feat information, path, name reserved to list
    double p = 0.;
    int tn = tx.nodeSize();
    char** Tfeatpath = (char**)malloc( sizeof(char*) * tn );
    FILE *T_fp = fopen( featPath, "r" );
    if ( T_fp ) {
        char buffer[1024] = {0};
        int len = 0;
        int i = 0;
        while( fscanf( T_fp, "%s", buffer) != EOF ) {
            len = strlen(buffer);
            char *tmp = (char*)malloc(len+1);
            memcpy( tmp, buffer, len );
            tmp[len] = 0;
            Tfeatpath[i++] = tmp;
            memset( buffer, 0, 1024 );
        }
        fclose(T_fp);
    }

/*
    CxSlideModule fx;
    fx.loadFeats( FalsefeatPath );
    int fn = fx.nodeSize();
    char** Ffeatpath = (char**)malloc( sizeof(char*) * fn );
    FILE *F_fp = fopen( FalsefeatPath, "r" );
    if ( F_fp ) {
        char buffer[1024] = {0};
        int len = 0;
        int i = 0;
        while( fscanf(F_fp, "%s", buffer) != EOF ) {
            len = strlen(buffer);
            char *tmp = (char*)malloc(len+1);
            memcpy( tmp, buffer, len );
            tmp[len] = 0;
            Ffeatpath[i++] = tmp;
            memset( buffer, 0, 1024 );
        }

        fclose(F_fp);
    }
*/
    //test
    int label = 0;
    fprintf( stderr, "//////////////////////// true sample //////////////////////// \n");
    int tnum = 0;
    int fnum = 0;
    for ( int i = 0; i < tn; i++ ) {
//        p = svm_predict( module.module(), module.nodes()[i] );
        label = 0;
        p = module.test( tx.nodes()[i], label );
        if ( p < 0 )
            fnum++;
        else
            tnum++;
        fprintf( stderr, "%s :  [%d] model probability %lf \n", Tfeatpath[i], label, p );
        free( Tfeatpath[i] );
    }
    free( Tfeatpath );
    fprintf( stderr, "test %d ture feature, accept rate = %lf\n", tn, (double)tnum / tn );
/*
    fprintf( stderr, "//////////////////////// false sample //////////////////////// \n");
    tnum = 0;
    fnum = 0;
    for ( int i = 0; i < fn; i++ ) {
//        p = svm_predict( module.module(), module.nodes()[i] );
        label = 0;
        p = module.test( fx.nodes()[i], label );
        if ( p < 0 )
            fnum++;
        else
            tnum++;
        fprintf( stderr, "%s :  [%d] model probability %lf \n", Ffeatpath[i], label, p );
        free( Ffeatpath[i] );
    }
    free( Ffeatpath );
    fprintf( stderr, "test %d false feature, reject rate = %lf\n", fn, (double)fnum / fn );
*/
    module.clear();
    tx.clear();
//    fx.clear();
}

int CXSlideNode::ref = 0;
int CXSlideNode::dataSize = sizeof(double) * CXSlideNode::slideNode_dim;
int CXSlideNode::gNumNode = 0;

//#include <QDebug>
CXSlideNode::CXSlideNode()
{
//    qDebug() << "new CXSlideNode " << this;
}

CXSlideNode::~CXSlideNode()
{
//    qDebug() << "destory CXSlideNode " << this;
    ref--;
    clear();
}

void CXSlideNode::clear()
{}

double* CXSlideNode::val()
{
    return m_data;
}

double CXSlideNode::distanceTo( CXDMNode *target )
{
    double dist = 0.;
    double* val = target->val();
    for ( int i = 0; i < slideNode_dim; i++ ) {
        double tmp = m_data[i] - val[i];
        tmp *= tmp;
        dist += tmp;
    }
    dist = sqrt( dist );
    return dist;
}

CXDMNode& CXSlideNode::operator =( CXDMNode &target )
{
    double* val = target.val();
    memcpy( m_data, val, dataSize );
    return *this;
}

CXDMNode& CXSlideNode::operator +=( CXDMNode &target )
{
    double* val = target.val();
    for ( int i = 0; i < slideNode_dim; i++ ) {
        m_data[i] += val[i];
    }
    return *this;
}

CXDMNode& CXSlideNode::operator +=( CXDMNode *target )
{
    double* val = target->val();
    for ( int i = 0; i < slideNode_dim; i++ ) {
        m_data[i] += val[i];
    }
    return *this;
}

CXDMNode& CXSlideNode::operator /=( double div )
{
    for ( int i = 0; i < slideNode_dim; i++ ) {
        m_data[i] /= div;
    }
    return *this;
}

void CXSlideNode::zero()
{
    memset( m_data, 0, dataSize );
}

CXDMNode* CXSlideNode::copy()
{
    CXSlideNode *pNew = _newNodes();
    double* val = pNew->val();
    memcpy( val, m_data, dataSize );
    return pNew;
}

CXDMNode* CXSlideNode::newNodes( int num )
{
    return _newNodes( num );
}

CXDMNode** CXSlideNode::newNodePointers( int num )
{
    if ( num > 0 ) {
        CXDMNode **list = CXDFMallocPoints( CXDMNode, num );
        for ( int i = 0; i < num; i++ )
            list[i] = _newNodes();
        return list;
    }
    return NULL;
}

void CXSlideNode::load( svm_node *snode )
{
    for ( int i = 0; i < slideNode_dim; i++ ) {
        m_data[i] = snode[i].value;
    }
    /*
    //slide area
    m_data[0] = snode[SLIDE_START_X].value; //position
    m_data[1] = snode[SLIDE_START_Y].value;
    m_data[2] = snode[SLIDE_END_X].value;
    m_data[3] = snode[SLIDE_END_Y].value;
    m_data[4] = snode[SLIDE_LDP_DIV].value; //div
    m_data[5] = snode[SLIDE_GLOBAL_DIV_MEAN].value; //div mean
    //slide arc
    m_data[6] = snode[SLIDE_FRONT_DISTANCE].value;//slide front distance
    m_data[7] = snode[SLIDE_FRONT_LENGTH].value;//slide front distance
    m_data[8] = snode[SLIDE_BACK_DISTANCE].value; //slide back distance
    m_data[9] = snode[SLIDE_BACK_LENGTH].value; //slide back distance
    //slide velocity
    m_data[10] = snode[SLIDE_GLOBAL_VEL].value; //slide velocity
   //press
    m_data[11] = snode[SLIDE_GLOBAL_PRESS].value; //slide pressure
    */
}

void CXSlideNode::load( double *featVals )
{
    memcpy( m_data, featVals, dataSize );
    /*
    //slide area
    m_data[0] = featVals[SLIDE_START_X]; //position
    m_data[1] = featVals[SLIDE_START_Y];
    m_data[2] = featVals[SLIDE_END_X];
    m_data[3] = featVals[SLIDE_END_Y];
    m_data[4] = featVals[SLIDE_LDP_DIV]; //div
    m_data[5] = featVals[SLIDE_GLOBAL_DIV_MEAN]; //div mean
    //slide arc
    m_data[6] = featVals[SLIDE_FRONT_DISTANCE];//slide front distance
    m_data[7] = featVals[SLIDE_FRONT_LENGTH];//slide front distance
    m_data[8] = featVals[SLIDE_BACK_DISTANCE]; //slide back distance
    m_data[9] = featVals[SLIDE_BACK_LENGTH]; //slide back distance
    //slide velocity
    m_data[10] = featVals[SLIDE_GLOBAL_VEL]; //slide velocity
   //press
    m_data[11] = featVals[SLIDE_GLOBAL_PRESS]; //slide pressure
    */
}

void CXSlideNode::setVal( double *pVal )
{
    if ( pVal )
        memcpy( m_data, pVal, dataSize );
}

CxSlideModule::CxSlideModule()
{
//    m_model = NULL;
    m_features = NULL;
    m_nodes = NULL;
//    m_featTags = NULL;
    m_numFeatures = 0;

    m_modules = NULL;
    m_nModule = 0;

    m_probs = NULL;
    m_meanVals = NULL;
    m_factors = NULL;
    m_bFactor = false;

    m_featDimSize = 0;
}

CxSlideModule::~CxSlideModule()
{
    clear();
}

void CxSlideModule::clear()
{
    releaseFeatures();
    releaseModule();

    CXDFFreeDouble( m_probs );
    CXDFFreeDouble( m_meanVals );
    CXDFFreeDouble( m_factors );
    m_bFactor = false;

    m_featDimSize = 0;
    m_kmeans.clear();
}

int CxSlideModule::load( const char *filePath )
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
        dVal = m_factors;
        for ( int i = 0; i < m_featDimSize; i++, dVal++ )
            fscanf( fp, "%lf", dVal );

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
            CXSlideNode *pNode  = CXSlideNode::_newNodes();
            pNode->setVal( pVal );
            pVal += m_kmeans.nDimSize;
            m_kmeans.means[i] = pNode;

        }//endfor
        memset( str, 0, 7 );
        fscanf( fp, "%s", str );
    }

    if ( !strcmp(str, "svm" ) ) {
        m_modules = CXDFMallocPoints( svm_model, m_nModule );
        char buffer[1024] = {0};
        LOGD("testStdNode pModel ncluster %d\n", m_kmeans.nCluster);
        for ( int k = 0; k < m_kmeans.nCluster; k++ ) {
            svm_model *pModel = NULL;
            memset( buffer, 0, 1024 );
            fscanf( fp, "%s", buffer );
            pModel = svm_load_model( buffer );
            m_modules[k] = pModel;
        }
    }

    fclose( fp );
    return 0;
}


int CxSlideModule::loadFeats( const char *filePath )
{
    clear();

    m_featDimSize = SLIDE_FEATURE_DIM_SIZE;

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
    int incr = m_featDimSize + 1;
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
        readFeature( featFile, pNode, SLIDE_FEATURE_DIM_SIZE );
        num++;
        pNode += incr;
        memset( pathBuffer, 0, 1024);
    }

    return 0;
}

void CxSlideModule::dynaCluster( int nCluster, int offset )
{
    if ( m_numFeatures < 1 || !m_features || ! m_nodes )
        return;

    CXSlideNode **datas = CXDFMallocPoints( CXSlideNode, m_numFeatures );
//    CXSlideNode *nodes = CXSlideNode::_newNodes( m_numFeatures );
//    CXSlideNode *ptr = nodes;
    svm_node **tmp = m_nodes;

    for ( int i = 0; i < m_numFeatures; i++, /*ptr++, */tmp++ ) {
//        datas[i] = ptr;
//        if ( *tmp )
//            ptr->load( (*tmp) );
        datas[i] = CXSlideNode::_newNodes();
        if ( *tmp )
            datas[i]->load( (*tmp) );
    }

    CXDmeans dms;
    int nK = nCluster;
//    int offset = 5;
    fprintf( stderr, "init k = %d, offset = %d ...\n", nK, offset );
    dms.dynamicCluster( (CXDMNode**)datas, m_numFeatures, nK, offset, &m_kmeans );
    m_kmeans.nDimSize = CXSlideNode::slideNode_dim;
    fprintf( stderr, "opt-cluster number: %d \n", m_kmeans.nCluster );
    for ( int i = 0; i < m_kmeans.nCluster; i++ )
        fprintf( stderr, "[%2d] cluser 's features number : %3d \n", i, m_kmeans.counts[i] );

    for ( int i = 0; i < m_numFeatures; i++ ) {
        delete datas[i];
        datas[i] = NULL;
    }
    CXDFFreePointers( datas );
//    delete[] nodes;
}

void CxSlideModule::train()
{
    if ( m_modules )    //reset model pointer list
        releaseModule();

    if ( m_kmeans.nCluster == 1 ) { //one class
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
    //declare svm parameter
    svm_problem prob;
    svm_parameter param;
    //y for tags; x for samples
    double *y = CXDFMallocDouble( max );
    svm_node **x = CXDFMallocPoints( svm_node, max );
    //prob's tags & sample features
    prob.x = x;
    prob.y = y;
    //param
    param.svm_type = ONE_CLASS; //类型
        param.kernel_type = RBF; //核函数
//        param.kernel_type = POLY; //核函数
    param.kernel_type = SIGMOID; //核函数
   // param.degree = 5; //多项式参数
    param.degree = 3; //多项式参数
    //param.gamma = 1.0f; //核函数参数
    param.gamma = 0.02f; //核函数参数
    //param.coef0 = 0.5f;
    param.coef0 = 0.0f;
    //param.nu = 0.5;
    param.nu = 0.01;
    param.cache_size = 100; //100mb buffer
    param.C = 1;
  //  param.eps = 1e-8;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    //init one class tags
    for ( int i = 0; i < max; i++ )
        y[i] = 1.;
    //svm_node dim size, end with signal index = -1
    int incr = m_featDimSize + 1;
    int *label = NULL;
    //train each cluster features => svm one class model
    for ( int k = 0; k < m_kmeans.nCluster; k++ ) {
        prob.l = m_kmeans.counts[k]; //number of sample features
        label = m_kmeans.labels; //get all features class labels
        int num = 0; //reserve current k cluser features count
        for ( int i = 0; i < m_numFeatures; i++, label++ ) { //search all features
            if ( *label == k ) { //when feature label = current train cluster index
                x[num] = m_features + i * incr; //get start pointer
                num++;
                if ( num == prob.l )
                    break;
            }
        }
        //message
        fprintf( stderr, "train %d svm model, count = %d ...\n", k, prob.l );
        const char* message  = svm_check_parameter( &prob, &param );
        if ( message )
            fprintf( stderr, "train %d svm model, message %s\n", k, message );

        svm_model *pModel = trainModel( prob, param );
        m_modules[k] = pModel;
    }

    CXDFFreeDouble( y );
    CXDFFreePointers( x );
}

svm_model* CxSlideModule::trainModel( svm_problem &prob, svm_parameter &param )
{
    const char* message  = svm_check_parameter( &prob, &param );
    if ( message )
        fprintf( stderr, "%s", message );
    svm_model *pModel = svm_train( &prob, &param );
    return pModel;
}


void CxSlideModule::trainOne()
{
    m_nModule = 1;
    m_modules = CXDFMallocPoints( svm_model, m_kmeans.nCluster );
    svm_problem prob;
    svm_parameter param;
    double *y = CXDFMallocDouble( m_numFeatures );
    for ( int i = 0; i < m_numFeatures; i++ )
        y[i] = 1.;
    prob.l = m_numFeatures;
    prob.x = m_nodes;
    prob.y = y;

    param.svm_type = ONE_CLASS; //类型
    param.kernel_type = RBF; //核函数
//    param.kernel_type = POLY; //核函数
//    param.kernel_type = SIGMOID; //核函数
  //  param.degree = 5; //多项式参数
    param.degree = 3; //多项式参数
   // param.gamma = 1.0f; //核函数参数
  //  param.gamma = 0.2f; //核函数参数
    param.gamma = 0.02f; //核函数参数
  //  param.coef0 = 0.5f;
    param.coef0 = 0.0f;
    //param.nu = 0.5;
    param.nu = 0.01;
    param.cache_size = 100; //100mb buffer
    param.C = 1;
  //  param.eps = 1e-8;
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

    CXDFFreeDouble( y );
    return;
}

void CxSlideModule::factored( int dimSize )
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
        const double MIN_VAR = 1E-10;
        m_meanVals[i] /= (double)m_numFeatures;
        fprintf( stderr, "feat[%2d] : min %lf ; max %lf ; span %lf; scale : %lf; mean: %lf    ",
                 i, minBuffer[i], maxBuffer[i], maxBuffer[i] - minBuffer[i], slide_feature_scale[i], m_meanVals[i] );
        m_factors[i] = maxBuffer[i] - minBuffer[i];
        m_factors[i] /= slide_feature_scale[i];
        if (m_factors[i] < DBL_MIN)
            m_factors[i] = DBL_MAX;
        fprintf( stderr, "factor[%2d] : %lf \n", i, m_factors[i] );
    }

    //ceshi mean, var
    feat = m_features;
    memset( minBuffer, 0, size );
    while ( feat < limit ) {
        if ( feat->index < 0 ) {
            feat++;
            continue;
        }
        double delta = feat->value - means[feat->index];
        delta *= delta;
        min[feat->index] += delta;
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

double CxSlideModule::test( double *val, int &label )
{
    int size = m_featDimSize + 1;
    svm_node *x = new svm_node[ size ];


    if ( m_bFactor && m_factors && m_meanVals ) {
        //ceshi
        double dist = 0.;
        for ( int i = 0; i < m_featDimSize; i++ )  {
            x[i].index = i + 1;
            x[i].value -= m_meanVals[i];

            //ceshi
            {
            double tmp = x[i].value;
            tmp *= tmp;
            dist += tmp;
            }

            x[i].value = val[i] / m_factors[i];
        }
        x[m_featDimSize].index = -1;
        x[m_featDimSize].index = 0.;

        //ceshi
        fprintf( stderr, "global dist offset :  %lf\n", dist );
    }

    double ret =  testStdNode( x, label );
    delete[] x;
    return ret;
}

double CxSlideModule::test( svm_node *x, int &label )
{
    if ( m_bFactor && m_factors && m_meanVals ) {
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
            x[i].value /= m_factors[i];
        }

        //ceshi
        fprintf( stderr, "global dist offset :  %lf\n", dist );

    }

    double ret =  testStdNode( x, label );
    return ret;
}

double CxSlideModule::testStdNode( svm_node *x, int &label )
{
//    int label = 0;
    LOGD("testStdNode %d\n", label);
    label = 0;
    if ( m_nModule > 1 ) {
        CXSlideNode *pNode = CXSlideNode::_newNodes();
        pNode->load( x );
        CXDmeans::getLabel( pNode, &m_kmeans, m_kmeans.nCluster, label );
        delete pNode;
    }
    LOGD("testStdNode pModel %p\n", m_modules);
    svm_model *pModel = m_modules[label];
    LOGD("testStdNode pModel %p\n", pModel);
    double ret = svm_predict( pModel, x );
    return ret;
}

bool CxSlideModule::checkFeatDim( svm_node *x )
{
    return  !isEqualF( x[m_featDimSize].index, -1. );
}

void CxSlideModule::save( const char *filePath )
{
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

void CxSlideModule::saveModule( const char *filePath, svm_model *pModel )
{
    svm_save_model( filePath, pModel );
}

void CxSlideModule::releaseFeatures()
{
    if ( m_features ) {
        CXDFFREE( m_features );
        CXDFFreePointers( m_nodes );
    }
//    CXDFFREE( m_featTags );
    m_numFeatures = 0;
}

void CxSlideModule::releaseModule()
{
//    if ( m_model ) {
//        svm_free_and_destroy_model(&m_model);
//    }
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

void CxSlideModule::readFeature( FILE *fp, svm_node *node, int size )
{
    int i = 0;
    int j = 1;
    double dval = 0.;
    while( i < size && fscanf( fp, "%lf", &dval ) != EOF ) {
        node[i].index = j;
        node[i].value = dval;
        dval = 0.f;
        i = j;
        j++;
    }
    node[i].index = -1;
}
