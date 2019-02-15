#include "cxslide.h"
#include "cxmath.h"
#include "cxsensordata.h"

float CXSlide::m_screenSize = 1920.f;
float CXSlide::m_pressureSize = 0.5f;
float CXSlide::m_velocitySize = 192000.f;
float CXSlide::m_accelerateSize = 1920000.f;

double slide_feature_scale[SLIDE_FEATURE_DIM_SIZE] = {
    1., //SLIDE_START_X = 0, //POSITION               位置,表达滑动在屏幕的位置区域  1, 2
    1., //SLIDE_START_Y,
    1., //SLIDE_END_X,
    1., //SLIDE_END_Y,
    1., //SLIDE_LDP_X,
    1., //SLIDE_LDP_Y,
    1., //SLIDE_LDP1_X,
    1., //SLIDE_LDP1_Y,
    1., //SLIDE_LDP2_X,
    1., //SLIDE_LDP2_Y,
    1., //SLIDE_LDP3_X,
    1., //SLIDE_LDP3_Y,
    1., //SLIDE_LDP4_X,
    1., //SLIDE_LDP4_Y,
    1., //SLIDE_LDP5_X,
    1., //SLIDE_LDP5_Y,
    1., //SLIDE_LDP6_X,
    1., //SLIDE_LDP6_Y,
    1., //SLIDE_LDP7_X,
    1., //SLIDE_LDP7_Y,
    1., //SLIDE_LDP8_X,
    1., //SLIDE_LDP8_Y,
    0.95, //SLIDE_FRONT_ROT, //ROTATION                 矢量方向旋转角度, 方向相关          3
    0.95, //SLIDE_BACK_ROT,
    0.95, //SLIDE_GLOBAL1_ROT,
    0.95, //SLIDE_GLOBAL2_ROT,
    0.95, //SLIDE_GLOBAL3_ROT,
    0.95, //SLIDE_GLOBAL4_ROT,
    0.95, //SLIDE_GLOBAL5_ROT,
    0.95, //SLIDE_GLOBAL6_ROT,
    0.95, //SLIDE_GLOBAL7_ROT,
    0.95, //SLIDE_GLOBAL_ROT,
    0.9, //SLIDE_GLOBAL_LENGTH, //                                 轨迹长度, 矢量长度                  4
//    0.9, //SLIDE_GLOBAL_DISTANCE,
    0.9, //SLIDE_FRONT_LENGTH,
    0.9, //SLIDE_FRONT_DISTANCE,
    0.9, //SLIDE_BACK_LENGTH,
    0.9, //SLIDE_BACK_DISTANCE,
    0.9, //SLIDE_BACK1_LENGTH,
    0.9, //SLIDE_BACK1_DISTANCE,
    0.9, //SLIDE_BACK2_LENGTH,
    0.9, //SLIDE_BACK2_DISTANCE,
    0.9, //SLIDE_BACK3_LENGTH,
    0.9, //SLIDE_BACK3_DISTANCE,
    0.9, //SLIDE_BACK4_LENGTH,
    0.9, //SLIDE_BACK4_DISTANCE,
    0.9, //SLIDE_BACK5_LENGTH,
    0.9, //SLIDE_BACK5_DISTANCE,
    0.9, //SLIDE_BACK6_LENGTH,
    0.9, //SLIDE_BACK6_DISTANCE,
    0.9, //SLIDE_BACK7_LENGTH,
    0.9, //SLIDE_BACK7_DISTANCE,
    1., //SLIDE_GLOBAL_VEL, //VELOCITY                    速度分布                                        5
    0.88, //SLIDE_FRONT_VEL,
    0.88, //SLIDE_BACK_VEL,
    0.88, //SLIDE_BACK1_VEL,
    0.88, //SLIDE_BACK2_VEL,
    0.88, //SLIDE_BACK3_VEL,
    0.88, //SLIDE_BACK4_VEL,
    0.88, //SLIDE_BACK5_VEL,
    0.88, //SLIDE_BACK6_VEL,
    0.88, //SLIDE_BACK7_VEL,
    1., //SLIDE_LDP_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_LDP1_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_LDP2_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_LDP3_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_LDP4_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_LDP5_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_LDP6_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_LDP7_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_LDP7_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    1., //SLIDE_GLOBAL_DIV_MEAN,
    0.88, //SLIDE_FRONT_DIV_MEAN,
    0.88, //SLIDE_BACK_DIV_MEAN,
    0.9, //SLIDE_GLOBAL_PRESS, //PRESSRURE             压力                                                  7
//    0.85, //SLIDE_FRONT_PRESS,
//    0.85, //SLIDE_BACK_PRESS,
    0.5, //SLIDE_GLOBAL_DIV_VAR, //DIVIATION VAR   偏移方差分布                                  8
    0.5, //SLIDE_FRONT_DIV_VAR,
    0.5, //SLIDE_BACK_DIV_VAR,
    0.5, //SLIDE_BACK1_DIV_VAR,
    0.5, //SLIDE_BACK2_DIV_VAR,
    0.5, //SLIDE_BACK3_DIV_VAR,
    0.5, //SLIDE_BACK4_DIV_VAR,
    0.5, //SLIDE_BACK5_DIV_VAR,
    0.5, //SLIDE_BACK6_DIV_VAR,
    0.5, //SLIDE_BACK7_DIV_VAR,
    0.5, //SLIDE_START_ACC, //0.5, //SLIDE ACCELERATE         特征点加速度                                  9
    0.5, //SLIDE_END_ACC,
    0.5, //SLIDE_LDP_ACC,
    0.5, //SLIDE_LDP1_ACC,
    0.5, //SLIDE_LDP2_ACC,
    0.5, //SLIDE_LDP3_ACC,
    0.5, //SLIDE_LDP4_ACC,
    0.5, //SLIDE_LDP5_ACC,
    0.5, //SLIDE_LDP6_ACC,
    0.5, //SLIDE_LDP7_ACC,
    0.5, //SLIDE_LDP8_ACC,

//    0.5, //SLIDE_GLOBAL_PRESS_VAR, //PRESSURE VAR
//    0.5, //SLIDE_FRONT_PRESS_VAR,
//    0.5, //SLIDE_BACK_PRESS_VAR
    0.8, //SLIDE_GLOBAL_GRAV_MEAN_X, //GLOBAL GRAVITY MEAN                                     10      传感器数据分布
    0.8, //SLIDE_GLOBAL_GRAV_MEAN_Y,
    0.8, //SLIDE_GLOBAL_GRAV_MEAN_Z,
//    0.8, //SLIDE_GLOBAL_ACCS_MEAN_X, //GLOBAL ACCELERATE MEAN
//    0.8, //SLIDE_GLOBAL_ACCS_MEAN_Y,
//    0.8, //SLIDE_GLOBAL_ACCS_MEAN_Z,
    0.8, //SLIDE_GLOBAL_GYRO_MEAN_X, //GLOBAL GYRO MEAN
    0.8, //SLIDE_GLOBAL_GYRO_MEAN_Y,
    0.8, //SLIDE_GLOBAL_GYRO_MEAN_Z,
//    0.6, //SLIDE_FRONT_GRAV_MEAN_X, //RRONT GRAV MEAN                                            11
//    0.6, //SLIDE_FRONT_GRAV_MEAN_Y,
//    0.6, //SLIDE_FRONT_GRAV_MEAN_Z,
//    0.6, //SLIDE_FRONT_ACCS_MEAN_X, //RRONT ACCS MEAN
//    0.6, //SLIDE_FRONT_ACCS_MEAN_Y,
//    0.6, //SLIDE_FRONT_ACCS_MEAN_Z,
//    0.6, //SLIDE_FRONT_GYRO_MEAN_X, //RRONT GYRO MEAN
//    0.6, //SLIDE_FRONT_GYRO_MEAN_Y,
//    0.6, //SLIDE_FRONT_GYRO_MEAN_Z,
//    0.6, //SLIDE_BACK_GRAV_MEAN_X, //BACK GRAV MEAN
//    0.6, //SLIDE_BACK_GRAV_MEAN_Y,
//    0.6, //SLIDE_BACK_GRAV_MEAN_Z,
//    0.6, //SLIDE_BACK_ACCS_MEAN_X, // BACK ACCS MEAN
//    0.6, //SLIDE_BACK_ACCS_MEAN_Y,
//    0.6, //SLIDE_BACK_ACCS_MEAN_Z,
//    0.6, //SLIDE_BACK_GYRO_MEAN_X,// BACK GYRO MEAN
//    0.6, //SLIDE_BACK_GYRO_MEAN_Y,
//    0.6, //SLIDE_BACK_GYRO_MEAN_Z,
    0.5, //SLIDE_GLOBAL_GRAV_VAR_X, //GLOBAL GRAVITY VAR                                               12
    0.5, //SLIDE_GLOBAL_GRAV_VAR_Y,
    0.5, //SLIDE_GLOBAL_GRAV_VAR_Z,
    0.5, //SLIDE_GLOBAL_ACCS_VAR_X, //GLOBAL ACCELERATE VAR
    0.5, //SLIDE_GLOBAL_ACCS_VAR_Y,
    0.5, //SLIDE_GLOBAL_ACCS_VAR_Z,
    0.5, //SLIDE_GLOBAL_GYRO_VAR_X, //GLOBAL GYRO VAR
    0.5, //SLIDE_GLOBAL_GYRO_VAR_Y,
    0.5, //SLIDE_GLOBAL_GYRO_VAR_Z,
//    0.4, //SLIDE_FRONT_GRAV_VAR_X, //RRONT GRAV VAR                                                      13
//    0.4, //SLIDE_FRONT_GRAV_VAR_Y,
//    0.4, //SLIDE_FRONT_GRAV_VAR_Z,
//    0.4, //SLIDE_FRONT_ACCS_VAR_X, //RRONT ACCS VAR
//    0.4, //SLIDE_FRONT_ACCS_VAR_Y,
//    0.4, //SLIDE_FRONT_ACCS_VAR_Z,
//    0.4, //SLIDE_FRONT_GYRO_VAR_X, //RRONT GYRO VAR
//    0.4, //SLIDE_FRONT_GYRO_VAR_Y,
//    0.4, //SLIDE_FRONT_GYRO_VAR_Z,
//    0.4, //SLIDE_BACK_GRAV_VAR_X, //BACK GRAV VAR
//    0.4, //SLIDE_BACK_GRAV_VAR_Y,
//    0.4, //SLIDE_BACK_GRAV_VAR_Z,
//    0.4, //SLIDE_BACK_ACCS_VAR_X, // BACK ACCS VAR
//    0.4, //SLIDE_BACK_ACCS_VAR_Y,
//    0.4, //SLIDE_BACK_ACCS_VAR_Z,
//    0.4, //SLIDE_BACK_GYRO_VAR_X,// BACK GYRO VAR
//    0.4, //SLIDE_BACK_GYRO_VAR_Y,
//    0.4, //SLIDE_BACK_GYRO_VAR_Z,

};

int featSlideSample( CXSlide *slide, sensor_sample *sample, const char *featPath ) {
 //   if ( sample->isEmpty() )
  //      return -1;

    if ( slide->loadSample( sample ) )
        return -1;
    fprintf(stderr, "loadsample finished \n");
//    const char *featPath = "/home/xizhi/UnifyID/feature/slide/train/xizhi/";
//    const char *featPath = "/home/xizhi/UnifyID/feature/slide/train/shabin/";
    int pathLength = strlen(featPath);
    char buffer[1024] = {0};
//    char outputPath[1024] = {0};
    memset( buffer, 0, 1024 );
    memcpy( buffer, featPath, pathLength );
//    memset( outputPath, 0, 1024 );
//    memcpy( outputPath, featPath, pathLength );
//    const char *featListExt = "zzzoutput.featurs";
//    char *ptr = outputPath + pathLength;
//    memcpy( ptr, featListExt, strlen(featListExt) );

    char *ptr = buffer + pathLength;
    char *fpathStr = sample->key.getKeyStr() ;
    int keyLen = strlen( fpathStr );
    memcpy( ptr, fpathStr, keyLen );
    ptr += keyLen;
    const char *slideExt = ".slide";
    const char *featExt = ".feat";

    int slSize = strlen( slideExt );
    int ftSize = strlen( featExt );
    memcpy( ptr, featExt, ftSize );
    slide->saveFeat( buffer );
    memset( ptr, 0,ftSize );
    memcpy( ptr, slideExt, slSize );
   // slide->save( buffer );

    return 0;
}

void  featSlides( CXSensorSample *sampler, const char* samplePath, const char *featPath ) {

    int pathLength = strlen(featPath);

    char outputPath[1024] = {0};
    memset( outputPath, 0, 1024 );
    memcpy( outputPath, featPath, pathLength );
    const char *featListExt = "zzzoutput.featurs";
    char *ptr = outputPath + pathLength;
    memcpy( ptr, featListExt, strlen(featListExt) );

    char buffer[1024] = {0};
    memset( buffer, 0, 1024 );
    memcpy( buffer, featPath, pathLength );
    ptr = buffer + pathLength;

//    const char *slideExt = ".slide";
    const char *featExt = ".feat";
//    int slSize = strlen( slideExt );
    int ftSize = strlen( featExt );

    FILE *fp = fopen( outputPath, "w" );

    sampler->readSensor( samplePath );
//    sampler->sortMapSamples();
    sampler->sortTask();
    fprintf( stderr, "read sensor finished %d \n", sampler->m_task.nSample);
    CXSlide slide;
    for ( int i = 0; i < sampler->m_task.nSample; i++ ) {
        sensor_sample *sample = sampler->m_task.samples[i];
        if ( !featSlideSample( &slide, sample, featPath ) ) {
            if ( fp ) {
                char *fpathStr = sample->key.getKeyStr() ;
                int keyLen = strlen( fpathStr );
                memcpy( ptr, fpathStr, keyLen );
                char *tmp = ptr + keyLen;
                memcpy( tmp, featExt, ftSize );
                fprintf( fp, "%s\n", buffer );
                memset( ptr, keyLen + ftSize, 0 );
            }
        }
    }
    if ( fp )
        fclose( fp );
    CXSensorSample::gSampleMap.clear();
//    CXSensorSample::gSampleBuffer.clear();
}

void  ceshiSlide() {
//    const char* samplePath = "/home/xizhi/UnifyID/input/samples.info";
//    const char *featPath = "/home/xizhi/UnifyID/feature/slide/train/xizhi/";

//    const char* shabinPath = "/home/xizhi/UnifyID/input/shabin.info";
////    const char *shabinfeatPath = "/home/xizhi/UnifyID/feature/slide/train/shabin/";
//    const char *shabinfeatPath = "/home/xizhi/UnifyID/feature/slide/test/shabin/";

//    const char* gaoPath = "/home/xizhi/UnifyID/input/gao.info";
////    const char *gaofeatPath = "/home/xizhi/UnifyID/feature/slide/train/gao/";
//    const char *gaofeatPath = "/home/xizhi/UnifyID/feature/slide/test/gao/";

    const char* trainPath = "/home/xizhi/ceshi/train3/slide-sample.info";
    const char *trainfeatPath = "/home/xizhi/ceshi/train3/feat/";
//    const char* testPath = "/home/xizhi/ceshi/test3/slide-sample.info";
//    const char *testfeatPath = "/home/xizhi/ceshi/test3/feat/";

//    const char* samplePath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/input/shabin.info";
  //  const char* samplePath = "/home/binsha/tmp/latest-unifyid/slide-sample.info";
   // const char* samplePath = "/home/binsha/tmp/test-unifyid/slide-sample.info";
    const char* samplePath = "/home/binsha/tmp/left-unifyid-2/slide-sample.info";

    const char *featPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/train/shabin/";

  //  const char* samplePath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/input/shabin.info";

    const char *xizhifeatPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/train/xizhi/";
    const char *chenshuaifeatPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/train/shabin-test/";
    const char *testfeatPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/test/shabin-left-2/";

 //   const char *chenshuaifeatPath = "/home/binsha/tmp/latest-unifyid/";

    const char* shabinPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/input/shabin-test.info";

//    const char *shabinfeatPath = "/home/xizhi/UnifyID/feature/slide/train/shabin/";
    const char *shabinfeatPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/test/shabin/";
    const char *xizhitestfeatPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/test/xizhi/";
    const char *chenshuaitestfeatPath = "/home/binsha/work/SyberAI/unifyid/unify-id-data/UnifyID/feature/slide/test/chenshuai/";

    CXSensorSample sampler;

    featSlides( &sampler, samplePath, testfeatPath );
    sampler.clear();

 //   featSlides( &sampler, shabinPath, shabinfeatPath );
 //   sampler.clear();

//    featSlides( &sampler, gaoPath, gaofeatPath );
//    sampler.clear();

 //   featSlides( &sampler, shabinPath, chenshuaitestfeatPath );
//    sampler.clear();

//    featSlides( &sampler, testPath, testfeatPath );
 //   sampler.clear();

}


CXSlide::CXSlide()
{
}

CXSlide::~CXSlide()
{}

void CXSlide::clear()
{}

int CXSlide::loadSample( sensor_sample *sample )
{
    fprintf(stderr, "loadsample ing \n");
    fprintf(stderr, "loadsample ing sample->sensors[SENSOR_T] %d\n", sample->sensors[SENSOR_ACCS].sdata.nData);
    sdata_packet *ptouch = &sample->sensors[SENSOR_TOUCH];
    if ( ptouch->isEmpty() ) {
        fprintf( stderr, "sensor samples is empty!" );
        return -1;
    }

    sensor_data *sdata  = &ptouch->sdata;
    if ( sdata->nData < 3 ) {
        fprintf( stderr, "not have enough sensor samples, number of sample must > 3 !" );
        return -1;
    }
/*
    //temporarily output sensor value//
    int sum = sdata->nData-1;
    //cxSlidePoint m_points[slide_numPoint];
    m_output_points = new cxSlidePoint[sum];
    cxSlidePoint * tmp_points = m_output_points;
    sum_po = sum;
    for (int i=0;i<=sum;i++) {
        //cxSlidePoint pt;
        slidePoint( tmp_points++, sample, i );

    }

    return 0;
    // end output //
*/

    accsSubGrav( sample );

    cxSlidePoint *pt = m_points;
    int pos = sdata->nData-1;

    cxPointF start;
    cxPointF end;
    cxPointF ldpPt[9];
    int ldps[9];
    cxPointF ldpPt4;
    cxPointF ldpPt5;
    cxPointF ldpPt6;
    cxPointF ldpPt7;
    start.fx = sdata->tdatas[0].val[1];
    start.fy = sdata->tdatas[0].val[2];
    end.fx = sdata->tdatas[pos].val[1];
    end.fy = sdata->tdatas[pos].val[2];
    fprintf(stderr, "loadsample staged \n");

    slidePoint( pt++, sample, 0 );
    fprintf(stderr, "loadsample staged \n");

    slidePoint( pt++, sample, pos );
    fprintf(stderr, "loadsample staged \n");

    cxSlideStage *front = m_stage;
    cxSlideStage *back = front + 1;
    cxSlideStage *stage = back + 1;
    fprintf(stderr, "loadsample staged \n");

    cxSlideStage * stage1 = stage + 1;
    cxSlideStage * stage2 = stage1 + 1;
    cxSlideStage * stage3 = stage2 + 1;
    cxSlideStage * stage4 = stage3 + 1;
    cxSlideStage * stage5 = stage4 + 1;
    cxSlideStage * stage6 = stage5 + 1;
    cxSlideStage * stage7 = stage6 + 1;

    fprintf(stderr, "loadsample staged \n");


{
    cxLineDesc line;
    int ldp = 0;
    float *dist = (float*)malloc( sizeof(float) * sdata->nData );
    if ( isEqualF( start.fx, end.fx ) ) {
        line.type = line_vertical;
        line.fx = start.fx;
        if ( end.fy > start.fy )
            line.fy = 1.;
        else
            line.fy = -1.;
    } else if ( isEqualF( start.fy, end.fy ) ) {
        line.type = line_horizontal;
        line.fy = start.fy;
        if ( end.fx > start.fx )
            line.fx = 1.;
        else
            line.fx = -1.;
    } else {
        line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
        line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
    }

    ldp = findLdpPosition( sdata, dist, line );
    ldps[0] = ldp;
    m_sDesc.touch.ldpDistance[0] = dist[ldp];
    m_ldpDist[0] = dist[ldp];

    ldpPt[0].fx = sdata->tdatas[ldp].val[1];
    ldpPt[0].fy = sdata->tdatas[ldp].val[2];
    slidePoint( pt++, sample, ldp );

    CXDFFREE( dist );

}

    {
        end.fx = ldpPt[0].fx;
        end.fy = ldpPt[0].fy;
        cxLineDesc line;
        int ldp = 0;
        float *dist = (float*)malloc( sizeof(float) * sdata->nData );
        if ( isEqualF( start.fx, end.fx ) ) {
            line.type = line_vertical;
            line.fx = start.fx;
            if ( end.fy > start.fy )
                line.fy = 1.;
            else
                line.fy = -1.;
        } else if ( isEqualF( start.fy, end.fy ) ) {
            line.type = line_horizontal;
            line.fy = start.fy;
            if ( end.fx > start.fx )
                line.fx = 1.;
            else
                line.fx = -1.;
        } else {
            line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
            line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
        }

        ldp = findLdpPosition( sdata, dist, line ,0, ldps[0]);
        ldps[1] = ldp;
        m_sDesc.touch.ldpDistance[1] = dist[ldp];
        m_ldpDist[1] = dist[ldp];

        ldpPt[1].fx = sdata->tdatas[ldp].val[1];
        ldpPt[1].fy = sdata->tdatas[ldp].val[2];
        slidePoint( pt++, sample, ldp );

        front->rotation = rotation( m_points[slide_start].touch.desc.direct,
                                           m_points[slide_ldp].touch.desc.direct );
        slideStage( front, sample, dist, 0, ldp );

        stage2->rotation = rotation( m_points[slide_touch4].touch.desc.direct,
                                                                                m_points[slide_touch].touch.desc.direct );
        slideStage( stage2, sample, dist, ldps[1], ldps[0] );
        CXDFFREE( dist );

    }

    {
        start.fx = ldpPt[0].fx;
        start.fy = ldpPt[0].fy;
        end.fx = sdata->tdatas[pos].val[1];
        end.fy = sdata->tdatas[pos].val[2];
        cxLineDesc line;
        int ldp = 0;
        float *dist = (float*)malloc( sizeof(float) * sdata->nData );
        if ( isEqualF( start.fx, end.fx ) ) {
            line.type = line_vertical;
            line.fx = start.fx;
            if ( end.fy > start.fy )
                line.fy = 1.;
            else
                line.fy = -1.;
        } else if ( isEqualF( start.fy, end.fy ) ) {
            line.type = line_horizontal;
            line.fy = start.fy;
            if ( end.fx > start.fx )
                line.fx = 1.;
            else
                line.fx = -1.;
        } else {
            line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
            line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
        }

        ldp = findLdpPosition( sdata, dist, line ,ldps[0], pos);
        ldps[2] = ldp;
        m_sDesc.touch.ldpDistance[2] = dist[ldp];
        m_ldpDist[2] = dist[ldp];

        ldpPt[2].fx = sdata->tdatas[ldp].val[1];
        ldpPt[2].fy = sdata->tdatas[ldp].val[2];
        slidePoint( pt++, sample, ldp );

        back->rotation = rotation( m_points[slide_ldp].touch.desc.direct,
                                           m_points[slide_end].touch.desc.direct );
        slideStage( back, sample, dist, ldp, pos );

        stage3->rotation = rotation( m_points[slide_touch].touch.desc.direct,
                                                                                m_points[slide_touch5].touch.desc.direct );
        slideStage( stage3, sample, dist, ldps[0], ldps[2] );

        stage4->rotation = rotation( m_points[slide_touch5].touch.desc.direct,
                                                                                m_points[slide_end].touch.desc.direct );
        slideStage( stage4, sample, dist, ldps[2], pos );
        CXDFFREE( dist );

    }

    {
        start.fx = ldpPt[2].fx;
        start.fy = ldpPt[2].fy;
        end.fx = sdata->tdatas[pos].val[1];
        end.fy = sdata->tdatas[pos].val[2];
        cxLineDesc line;
        int ldp = 0;
        float *dist = (float*)malloc( sizeof(float) * sdata->nData );
        if ( isEqualF( start.fx, end.fx ) ) {
            line.type = line_vertical;
            line.fx = start.fx;
            if ( end.fy > start.fy )
                line.fy = 1.;
            else
                line.fy = -1.;
        } else if ( isEqualF( start.fy, end.fy ) ) {
            line.type = line_horizontal;
            line.fy = start.fy;
            if ( end.fx > start.fx )
                line.fx = 1.;
            else
                line.fx = -1.;
        } else {
            line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
            line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
        }

        ldp = findLdpPosition( sdata, dist, line ,ldps[2], pos);
        ldps[3] = ldp;
        m_sDesc.touch.ldpDistance[3] = dist[ldp];
        m_ldpDist[3] = dist[ldp];

        ldpPt[3].fx = sdata->tdatas[ldp].val[1];
        ldpPt[3].fy = sdata->tdatas[ldp].val[2];
        slidePoint( pt++, sample, ldp );
        CXDFFREE( dist );

    }

    {
        start.fx = sdata->tdatas[0].val[1];
        start.fy = sdata->tdatas[0].val[2];
        end.fx = ldpPt[1].fx;
        end.fy = ldpPt[1].fy;
        cxLineDesc line;
        int ldp = 0;
        float *dist = (float*)malloc( sizeof(float) * sdata->nData );
        if ( isEqualF( start.fx, end.fx ) ) {
            line.type = line_vertical;
            line.fx = start.fx;
            if ( end.fy > start.fy )
                line.fy = 1.;
            else
                line.fy = -1.;
        } else if ( isEqualF( start.fy, end.fy ) ) {
            line.type = line_horizontal;
            line.fy = start.fy;
            if ( end.fx > start.fx )
                line.fx = 1.;
            else
                line.fx = -1.;
        } else {
            line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
            line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
        }

        ldp = findLdpPosition( sdata, dist, line ,0, ldps[1]);
        ldps[4] = ldp;
        m_sDesc.touch.ldpDistance[4] = dist[ldp];
        m_ldpDist[4] = dist[ldp];

        ldpPt[4].fx = sdata->tdatas[ldp].val[1];
        ldpPt[4].fy = sdata->tdatas[ldp].val[2];
        slidePoint( pt++, sample, ldp );

        stage1->rotation = rotation( m_points[slide_start].touch.desc.direct,
                                                                                m_points[slide_touch4].touch.desc.direct );
        slideStage( stage1, sample, dist, 0, ldps[1] );
        CXDFFREE( dist );

    }

    {
        start.fx = sdata->tdatas[0].val[1];
        start.fy = sdata->tdatas[0].val[2];
        end.fx = ldpPt[4].fx;
        end.fy = ldpPt[4].fy;
        cxLineDesc line;
        int ldp = 0;
        float *dist = (float*)malloc( sizeof(float) * sdata->nData );
        if ( isEqualF( start.fx, end.fx ) ) {
            line.type = line_vertical;
            line.fx = start.fx;
            if ( end.fy > start.fy )
                line.fy = 1.;
            else
                line.fy = -1.;
        } else if ( isEqualF( start.fy, end.fy ) ) {
            line.type = line_horizontal;
            line.fy = start.fy;
            if ( end.fx > start.fx )
                line.fx = 1.;
            else
                line.fx = -1.;
        } else {
            line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
            line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
        }

        ldp = findLdpPosition( sdata, dist, line ,0, ldps[4]);
        ldps[5] = ldp;
        m_sDesc.touch.ldpDistance[5] = dist[ldp];
        m_ldpDist[5] = dist[ldp];

        ldpPt[5].fx = sdata->tdatas[ldp].val[1];
        ldpPt[5].fy = sdata->tdatas[ldp].val[2];
        slidePoint( pt++, sample, ldp );

        stage5->rotation = rotation( m_points[slide_start].touch.desc.direct,
                                                                                m_points[slide_touch5].touch.desc.direct );
        slideStage( stage5, sample, dist, 0, ldps[5] );
        CXDFFREE( dist );

    }


    {
        start.fx = ldpPt[1].fx;
        start.fy = ldpPt[1].fy;
        end.fx = ldpPt[0].fx;
        end.fy = ldpPt[0].fy;
        cxLineDesc line;
        int ldp = 0;
        float *dist = (float*)malloc( sizeof(float) * sdata->nData );
        if ( isEqualF( start.fx, end.fx ) ) {
            line.type = line_vertical;
            line.fx = start.fx;
            if ( end.fy > start.fy )
                line.fy = 1.;
            else
                line.fy = -1.;
        } else if ( isEqualF( start.fy, end.fy ) ) {
            line.type = line_horizontal;
            line.fy = start.fy;
            if ( end.fx > start.fx )
                line.fx = 1.;
            else
                line.fx = -1.;
        } else {
            line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
            line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
        }

        ldp = findLdpPosition( sdata, dist, line ,ldps[1], ldps[0]);
        ldps[6] = ldp;
        m_sDesc.touch.ldpDistance[6] = dist[ldp];
        m_ldpDist[6] = dist[ldp];

        ldpPt[6].fx = sdata->tdatas[ldp].val[1];
        ldpPt[6].fy = sdata->tdatas[ldp].val[2];
        slidePoint( pt++, sample, ldp );

        stage6->rotation = rotation( m_points[slide_touch4].touch.desc.direct,
                                                                                m_points[slide_touch8].touch.desc.direct );
        slideStage( stage6, sample, dist, ldps[1], ldps[6] );
        CXDFFREE( dist );

    }

    {
        start.fx = ldpPt[0].fx;
        start.fy = ldpPt[0].fy;
        end.fx = ldpPt[2].fx;
        end.fy = ldpPt[2].fy;
        cxLineDesc line;
        int ldp = 0;
        float *dist = (float*)malloc( sizeof(float) * sdata->nData );
        if ( isEqualF( start.fx, end.fx ) ) {
            line.type = line_vertical;
            line.fx = start.fx;
            if ( end.fy > start.fy )
                line.fy = 1.;
            else
                line.fy = -1.;
        } else if ( isEqualF( start.fy, end.fy ) ) {
            line.type = line_horizontal;
            line.fy = start.fy;
            if ( end.fx > start.fx )
                line.fx = 1.;
            else
                line.fx = -1.;
        } else {
            line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
            line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
        }

        ldp = findLdpPosition( sdata, dist, line ,ldps[0], ldps[2]);
        ldps[7] = ldp;
        m_sDesc.touch.ldpDistance[7] = dist[ldp];
        m_ldpDist[7] = dist[ldp];

        ldpPt[7].fx = sdata->tdatas[ldp].val[1];
        ldpPt[7].fy = sdata->tdatas[ldp].val[2];
        slidePoint( pt++, sample, ldp );

        stage7->rotation = rotation( m_points[slide_touch].touch.desc.direct,
                                                                                m_points[slide_touch9].touch.desc.direct );
        slideStage( stage7, sample, dist, ldps[0], ldps[7] );
        CXDFFREE( dist );

    }

    {
        start.fx = ldpPt[4].fx;
        start.fy = ldpPt[4].fy;
        end.fx = sdata->tdatas[pos].val[1];
        end.fy = sdata->tdatas[pos].val[2];
        cxLineDesc line;
        int ldp = 0;
        float *dist = (float*)malloc( sizeof(float) * sdata->nData );
        if ( isEqualF( start.fx, end.fx ) ) {
            line.type = line_vertical;
            line.fx = start.fx;
            if ( end.fy > start.fy )
                line.fy = 1.;
            else
                line.fy = -1.;
        } else if ( isEqualF( start.fy, end.fy ) ) {
            line.type = line_horizontal;
            line.fy = start.fy;
            if ( end.fx > start.fx )
                line.fx = 1.;
            else
                line.fx = -1.;
        } else {
            line.k = ( end.fy - start.fy ) / (  end.fx - start.fx );
            line.b = start.fy - line.k*start.fx; //k*x + b = y; => b = y - k*x;
        }

        ldp = findLdpPosition( sdata, dist, line ,ldps[4], pos);
        ldps[8] = ldp;
        m_sDesc.touch.ldpDistance[8] = dist[ldp];
        m_ldpDist[8] = dist[ldp];

        ldpPt[8].fx = sdata->tdatas[ldp].val[1];
        ldpPt[8].fy = sdata->tdatas[ldp].val[2];
        slidePoint( pt, sample, ldp );

        stage7->rotation = rotation( m_points[slide_touch10].touch.desc.direct,
                                                                                m_points[slide_end].touch.desc.direct );
        slideStage( stage7, sample, dist, ldps[8], pos );
        CXDFFREE( dist );

    }


    fprintf(stderr, "loadsample ing end \n");




    stage->rotation = rotation( m_points[slide_start].touch.desc.direct,
                                                                            m_points[slide_end].touch.desc.direct );
//    slideStage( stage, sample, dist, 0, pos );
    vector( stage->vector, start, end );

    combineSensorStage( stage, front, back);

    normalize();
   // CXDFFREE( dist );

    return 0;
}

void CXSlide::save( const char *filepath )
{
    FILE *fp = fopen( filepath, "w" );
    if ( !fp )
        return;
    cxSlidePoint *pt = m_points;
    fprintf( fp, "<start>\n" );
    printPoint( fp, pt );
    pt++;
    fprintf( fp, "<end>\n" );
    printPoint( fp, pt );
    pt++;
    fprintf( fp, "<ldp>\n" );
    printPoint( fp, pt );

    cxSlideStage *stage = m_stage;
    fprintf( fp, "<front>\n" );
    printStage( fp, stage );
    stage++;
    fprintf( fp, "<back>\n" );
    printStage( fp, stage );
    stage++;
    fprintf( fp, "<stage>\n" );
    printStage( fp, stage );
    fclose( fp );
}

void CXSlide::saveFeat( const char *filepath )
{
    FILE *fp = fopen( filepath, "w" );
    if ( !fp )
        return;

    for ( int i = 0; i < SLIDE_FEATURE_DIM_SIZE; i++ ) {
        fprintf( fp, "%f\n", m_feature.fval[i] );
    }
/*
    for ( int i = 0; i <= sum_po; i++ ) {
        fprintf( fp, "%f\n", m_output_points[i].t);
        fprintf( fp, "%f\n", m_output_points[i].touch.std.fx );
        fprintf( fp, "%f\n", m_output_points[i].touch.std.fy );
        fprintf( fp, "%f\n", m_output_points[i].touch.std.pressure );
        fprintf( fp, "%f\n", m_output_points[i].grav[1].fx );
        fprintf( fp, "%f\n", m_output_points[i].grav[1].fy );
        fprintf( fp, "%f\n", m_output_points[i].grav[1].fz );
        fprintf( fp, "%f\n", m_output_points[i].accs[1].fx );
        fprintf( fp, "%f\n", m_output_points[i].accs[1].fy );
        fprintf( fp, "%f\n", m_output_points[i].accs[1].fz );
        fprintf( fp, "%f\n", m_output_points[i].gyro[1].fx );
        fprintf( fp, "%f\n", m_output_points[i].gyro[1].fy );
        fprintf( fp, "%f\n", m_output_points[i].gyro[1].fz );
        fprintf( fp, "\n");
    }
*/
    fclose( fp );
}

int CXSlide::loadFile( const char *filepath )
{
    FILE *fp = fopen( filepath, "r" );
    if ( !fp )
        return -1;
    char buffer[100] = { 0 };
    int ret = 0;
    const char *start = "<start>";
    const char *end = "<end>";
    const char *ldp = "<ldp>";
    const char *stage = "<stage>";
    const char *front = "<front>";
    const char *back = "<back>";
    while( fscanf( fp, "%s" ,buffer) != EOF ) {
        if ( !strncmp( buffer, start, 7 ) ) {
            ret = readPoint( fp, &m_points[slide_start] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, end, 5 ) ) {
            ret = readPoint( fp, &m_points[slide_end] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, ldp, 5 ) ) {
            ret = readPoint( fp, &m_points[slide_ldp] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, stage, 7 ) ) {
            ret = readStage( fp, &m_stage[slide_touch] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, front, 7 ) ) {
            ret = readStage( fp, &m_stage[slide_front] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, back, 6 ) ) {
            ret = readStage( fp, &m_stage[slide_back] );
            if ( ret )
                break;
        } else {
            ret = -1;
            fprintf( stderr, "unknow type, error!");
            break;
        }
        memset( buffer, 0, 100 );
    }

    fclose( fp );
    return ret;
}

int CXSlide::loadFeature( const char *filepath )
{
    FILE *fp = fopen( filepath, "r" );
    if ( !fp )
        return -1;
    int i = 0;
    while( fscanf( fp, "%f" , &m_feature.fval[i] ) != EOF ) {
        i++;
    }

    fclose( fp );

    if ( i != SLIDE_FEATURE_DIM_SIZE )
        return -1;

    return 0;
}

//
int CXSlide::findLdpPosition( sensor_data *sample, float *distances, cxLineDesc &line )
{
    int i = 2;
    int limit = sample->nData - 1;
    int ldp = 1;
    float dist = 0.;
    float tmp = 0.;
    sensor_TData *td = sample->tdatas;
    dist = distance(  td[ldp].val[1], td[ldp].val[2], line );
    distances[0] = 0.;
    distances[ldp] = dist;
    distances[limit] = 0.;
    while ( i < limit ) {
        tmp = distance(  td[i].val[1], td[i].val[2], line );
        distances[i] = tmp;
        if ( tmp > dist ) {
            ldp = i;
            dist = tmp;
        }
        i++;
    }

    return ldp;
}

int CXSlide::findLdpPosition( sensor_data *sample, float *distances, cxLineDesc &line , int start, int end)
{
    int i = 2;
    int limit = end;
    int ldp = 1;
    float dist = 0.;
    float tmp = 0.;
    sensor_TData *td = sample->tdatas;
    dist = distance(  td[start + ldp].val[1], td[start + ldp].val[2], line );
    distances[start] = 0.;
    distances[start+ldp] = dist;
    distances[limit] = 0.;
    while ( i+start < limit ) {
        tmp = distance(  td[i+start].val[1], td[i+start].val[2], line );
        distances[start+i] = tmp;
        if ( tmp > dist ) {
            ldp = i;
            dist = tmp;
        }
        i++;
    }

    return ldp +start;
}

void CXSlide::slidePoint( cxSlidePoint *pt, sensor_sample *sample, int index )
{
    slideTouchPoint( pt, &sample->sensors[SENSOR_TOUCH], index );
    fprintf(stderr, "loadsample staged \n");
    slideSensorPoint( pt, sample, index );
    fprintf(stderr, "loadsample staged \n");

}

void CXSlide::slideTouchPoint( cxSlidePoint *pt, sdata_packet *sample, int index )
{
    sensor_data *touch  = &sample->sdata;
    int limit = touch->nData - 1;
    float fx = 0.;
    float fy = 0.;
    int i = 0;
    int j = 0;
    int64_t t = 0;
    float ft = 0.f;
    float dist = 0.f;
    float val = 0.f;
    float tmp = 0.f;
    pt->touch.std.fx = touch->tdatas[index].val[1];                //x
    pt->touch.std.fy = touch->tdatas[index].val[2];                //y
    pt->touch.std.pressure = touch->tdatas[index].val[0];    //pressure
    pt->t = touch->tdatas[index].local_time;
    getPointShift( touch, index, fx, fy, dist, t );
    pt->touch.desc.direct = getTAngle( fx, fy );
    ft = CXDFSCALETIME( t );
    pt->touch.desc.vel = dist / ft;
    if ( !index ) {
        getPointShift( touch, 1, fx, fy, dist, t );
        ft = CXDFSCALETIME( t );
        val = dist / ft;
        val -= pt->touch.desc.vel;
        t = touch->tdatas[1].local_time - touch->tdatas[0].local_time;
    } else if ( index == limit ) {
        i = limit - 1;
        getPointShift( touch, i, fx, fy, dist, t );
        ft = CXDFSCALETIME( t );
        val = dist / ft;
        val = pt->touch.desc.vel - val;
        t = touch->tdatas[limit].local_time - touch->tdatas[i].local_time;
    } else {
        i = index - 1;
        j = index + 1;
        getPointShift( touch, j, fx, fy, dist, t );
        ft = CXDFSCALETIME( t );
        val = dist / ft;
        getPointShift( touch, i, fx, fy, dist, t );
        ft = CXDFSCALETIME( t );
        tmp = dist / ft;
        val -= tmp;
        t = touch->tdatas[j].local_time - touch->tdatas[i].local_time;
    }
    ft = CXDFSCALETIME( t );
    val /= ft;
    pt->touch.desc.acc = val;
}

void CXSlide::slideSensorPoint( cxSlidePoint *pt, sensor_sample *sample, int index )
{
    fprintf(stderr, "loadsample staged \n");
    int64_t t = sample->sensors[SENSOR_TOUCH].sdata.tdatas[index].local_time;
    sdata_packet *grav = sample->sensors + 1;
    sdata_packet *accs = sample->sensors + 2;
    sdata_packet *gyro = sample->sensors + 3;
    fprintf(stderr, "loadsample staged t %d %f \n",index, t);
    interSensorPoint( pt->grav, grav, t );
    fprintf(stderr, "loadsample staged \n");
    interSensorPoint( pt->accs, accs, t );
    interSensorPoint( pt->gyro, gyro, t );
}

void CXSlide::interSensorPoint( cxPoint3F *pt, sdata_packet *sample, int64_t t )
{
    fprintf(stderr, "interSensorP staged \n");
    sensor_data* data = &sample->sdata;
    int n = 0;
    int limit = data->nData - 1;
    while ( n < data->nData ) {
        if ( data->tdatas[n].local_time >= t )
            break;
        if ( n == limit )
            break;
        n++;
    }
    fprintf(stderr, "interSensorP staged 2 limit %d\n", limit);

    int i = 0;
    int j = 0;
    float dt = 0.;
    if ( n >= limit )
        n = limit;
    fprintf(stderr, "interSensorP staged 5\n");
    if ( !n || n == limit || data->tdatas[n].local_time == t ) {
        fprintf(stderr, "interSensorP staged 6 n = %d\n", n);
        pt->fx = data->tdatas[n].val[0];
        pt->fy = data->tdatas[n].val[1];
        pt->fz = data->tdatas[n].val[2];
        fprintf(stderr, "interSensorP staged 9\n");
        if ( !n ) {
            i = 0;
            j = 1;
        } else if ( n == limit) {
            i = limit -1;
            j = limit;
        } else {
            i = n - 1;
            j = n+1;
        }
        fprintf(stderr, "interSensorP staged 8\n");
        dt = data->tdatas[j].local_time - data->tdatas[i].local_time;
        dt = CXDFSCALETIME( dt );
        pt[1].fx = data->tdatas[j].val[0] - data->tdatas[i].val[0];
        pt[1].fy = data->tdatas[j].val[1] - data->tdatas[i].val[1];
        pt[1].fz = data->tdatas[j].val[2] - data->tdatas[i].val[2];
        fprintf(stderr, "interSensorP staged 7\n");
        pt[1].fx /= dt;
        pt[1].fy /= dt;
        pt[1].fz /= dt;
        fprintf(stderr, "interSensorP staged 4\n");
        return;
    }
    fprintf(stderr, "interSensorP staged 3\n");
    i = n -1;
    j = n;
    dt = data->tdatas[j].local_time - data->tdatas[i].local_time;
    float a = (float)( t - data->tdatas[i].local_time ) / dt;
    float b = 1. - a;
    pt->fx = data->tdatas[j].val[0]*a + data->tdatas[i].val[0]*b;
    pt->fy = data->tdatas[j].val[1]*a + data->tdatas[i].val[1]*b;
    pt->fz = data->tdatas[j].val[2]*a + data->tdatas[i].val[2]*b;
    pt[1].fx = data->tdatas[j].val[0] - data->tdatas[i].val[0];
    pt[1].fy = data->tdatas[j].val[1] - data->tdatas[i].val[1];
    pt[1].fz = data->tdatas[j].val[2] - data->tdatas[i].val[2];
    dt = CXDFSCALETIME( dt );
    pt[1].fx /= dt;
    pt[1].fy /= dt;
    pt[1].fz /= dt;
}

void CXSlide::getPointShift( sensor_data *touch, int index,
                                                    float &fx, float &fy,
                                                    float &dist, int64_t &t )
{
    int limit = touch->nData - 1;
    int i = 0;
    int j = 0;
    if ( !index ) {
        fx = touch->tdatas[1].val[1] - touch->tdatas[0].val[1];
        fy = touch->tdatas[1].val[2] - touch->tdatas[0].val[2];
        t = touch->tdatas[1].local_time - touch->tdatas[0].local_time;
        dist = CXDFDIST(fx, fy);
    } else if ( index == limit ) {
        i = limit - 1;
        fx = touch->tdatas[limit].val[1] - touch->tdatas[i].val[1];
        fy = touch->tdatas[limit].val[2] - touch->tdatas[i].val[2];
        t = touch->tdatas[limit].local_time - touch->tdatas[i].local_time;
        dist = CXDFDIST(fx, fy);
    } else {
        i = index - 1;
        j = index + 1;
        fx = touch->tdatas[index].val[1] - touch->tdatas[i].val[1];
        fy = touch->tdatas[index].val[2] - touch->tdatas[i].val[2];
        float dist0 = CXDFDIST(fx, fy);
        float ft0 = touch->tdatas[index].local_time - touch->tdatas[i].local_time;
        fx = touch->tdatas[j].val[1] - touch->tdatas[index].val[1];
        fy = touch->tdatas[j].val[2] - touch->tdatas[index].val[2];
        float dist1 = CXDFDIST(fx, fy);
        float ft1 = touch->tdatas[j].local_time - touch->tdatas[index].local_time;
        t = touch->tdatas[j].local_time - touch->tdatas[i].local_time;
        if ( isEqualF( ft0, ft1 ) ) {
            fx = touch->tdatas[j].val[1] - touch->tdatas[i].val[1];
            fy = touch->tdatas[j].val[2] - touch->tdatas[i].val[2];

        } else {
            float fmin = ft0;
            if ( fmin > ft1 ) {
                fmin = ft1;
            }
            float ix = touch->tdatas[i].val[1];
            float iy = touch->tdatas[i].val[2];
            float jx = touch->tdatas[j].val[1];
            float jy = touch->tdatas[j].val[2];
            if ( ft0 > fmin ) {
                float a = ( ft0 - fmin ) / ft0;
                float b = 1. - a;
                ix= b * touch->tdatas[i].val[1] + a * touch->tdatas[index].val[1];
                iy= b * touch->tdatas[i].val[2] + a * touch->tdatas[index].val[2];
            }
            if ( ft1 > fmin ) {
                float a = ( ft1 - fmin ) / ft0;
                float b = 1. - a;
                jx= b * touch->tdatas[j].val[1] + a * touch->tdatas[index].val[1];
                jy= b * touch->tdatas[j].val[2] + a * touch->tdatas[index].val[2];
            }

            fx = jx - ix;
            fy = jy - iy;
        }

        dist = dist0 + dist1;
    }
}
void CXSlide::slideStage( cxSlideStage *stage, sensor_sample *sample, float *dist,
                                                int start, int end )
{
    sdata_packet *ptouch = &sample->sensors[SENSOR_TOUCH];
    sensor_TData *tdatas = ptouch->sdata.tdatas;
    cxPointF spt( tdatas[start].val[1], tdatas[start].val[2] );
    cxPointF ept( tdatas[end].val[1], tdatas[end].val[2] );
    int64_t t0 = tdatas[start].local_time;
    int64_t t1 = tdatas[end].local_time;
    int64_t t = 0;
    float tmp = 0.f;
    float fx = 0.f;
    float fy = 0.f;
    float ft = 0.f;
    float fval = 0.f;
    int n = 0;
    int i = 0;
    //vector
    vector( stage->vector, spt, ept );
    //distance
    stage->distance = 0.;
    n = start;
    i = start+1;
    for ( ; i <= end; i++ ) {
        fx = tdatas[i].val[1] - tdatas[n].val[1];
        fy = tdatas[i].val[2] - tdatas[n].val[2];
        tmp = CXDFDIST( fx, fy );
        stage->distance += tmp;
        n = i;
    }
    //latency
    t = t1 - t0;
    stage->latency = CXDFSCALETIME( t );
    //velocity
    stage->vel = stage->distance / stage->latency;
    stage->deviation.mean = 0.f;
    stage->deviation.var = 0.f;
    stage->deviation.var2 = 0.f;
    //pressure
    stage->pressure.mean = 0.f;
    stage->pressure.var = 0.f;
    stage->pressure.var2 = 0.f;
    n = start;
    i = start+1;
    for ( ; i <= end; i++ ) {
        fval = ( dist[n]+dist[i] ) *0.5; //X
        t = tdatas[i].local_time - tdatas[n].local_time;
        ft = CXDFSCALETIME( t ); //N(time)
        tmp = fval * ft; //X*N
        stage->deviation.mean += tmp; //sigma( X )
        fval  *= fval; //X^2
        tmp = fval * ft; //X^2*N
        stage->deviation.var2 += tmp; //sigma( X^2 *N)

        //pressure
        fval = ( tdatas[i].val[0] + tdatas[n].val[0] ) * 0.5;
        tmp = fval * ft;
        stage->pressure.mean += tmp;
        fval *= fval;
        tmp = fval * ft;
        stage->pressure.var2 += tmp;

        n = i;
    }
    stage->deviation.mean /= stage->latency; //Mean = sigma( X ) / sigma( N(time) )
    tmp = stage->deviation.mean * stage->deviation.mean; //Mean^2
    stage->deviation.var = stage->deviation.var2;
    stage->deviation.var -= stage->latency * tmp; //sigma( X^2 ) - Mean * sigma( N(time) )
    stage->deviation.var /= stage->latency; //sigma( X-Mean)^2 / N(time)

    //pressure
    stage->pressure.mean /= stage->latency;
    tmp = stage->pressure.mean * stage->pressure.mean;
    stage->pressure.var = stage->pressure.var2;
    stage->pressure.var -= tmp * stage->latency;
    stage->pressure.var /= stage->latency;

    sensor_data *grav = &sample->sensors[SENSOR_GRAV].sdata;
    sensor_data *accs = &sample->sensors[SENSOR_ACCS].sdata;
    sensor_data *gyro = &sample->sensors[SENSOR_GYRO].sdata;

    int nGrav = slideSensorStage( &stage->grav, grav, t0, t1 );
    int nAccs = slideSensorStage( &stage->accs, accs, t0, t1 );
    int nGyro = slideSensorStage( &stage->gyro, gyro, t0, t1 );
    fprintf( stderr, "%s touch [%2d] : inner Gray[%2d]; Accs[%2d]; Gyro[%2d]\n",
                    sample->key.getKeyStr(),
                    ptouch->sdata.nData, nGrav, nAccs, nGyro );
}

int CXSlide::slideSensorStage( sDeviation3F *dev, sensor_data *sample, int64_t t0, int64_t t1)
{
    float tmp = 0.f;
    float ft = 0.f;
    float fval = 0.f;
    int64_t t = 0;
    dev->mean[0] = dev->mean[1] = dev->mean[2] = 0.f;
    dev->var[0] = dev->var[1] = dev->var[2] = 0.f;
    dev->var2[0] = dev->var2[1] = dev->var2[2] = 0.f;
    sensor_TData *end = sample->tdatas;
    sensor_TData *start = NULL;
    sensor_TData *limit = end + sample->nData;
    while ( end < limit && end->local_time < t1 ) {
        if ( end->local_time >= t0 && end->local_time <= t1 ) {
            if ( !start )
                start = end;
        }
        end++;
    }
    if ( !start )
        return -1;

    if ( start->local_time > t0 ) {
        t = start->local_time - t0;
        ft = CXDFSCALETIME( t ); //N(time)
        for ( int i =0; i < 3; i++  ) {
            fval = start->val[i];
            tmp = fval * ft; //X*N
            dev->mean[i] += tmp; //sigma( X*N )
            fval *= fval; //X^2;
            tmp = fval * ft; //X^2 * N
            dev->var2[i] += tmp; //sigma( X^2*N )
        }
    }
    int nPoint = end - start;
    end--;
    if ( end->local_time < t1 ) {
        t = t1 - end->local_time;
        ft = CXDFSCALETIME( t ); //N(time)
        for ( int i =0; i < 3; i++  ) {
            fval = end->val[i];
            tmp = fval * ft; //X*N
            dev->mean[i] += tmp; //sigma( X*N )
            fval *= fval; //X^2;
            tmp = fval * ft; //X^2 * N
            dev->var2[i] += tmp; //sigma( X^2*N )
        }
    }

    sensor_TData *last = start;
    start++;
    while ( start <= end ) {
        t = start->local_time - last->local_time;
        ft = CXDFSCALETIME( t ); //N(time)
        for ( int i =0; i < 3; i++  ) {
            fval = ( last->val[i] + start->val[i] ) * 0.5;
            tmp = fval * ft; //X*N
            dev->mean[i] += tmp; //sigma( X*N )
            fval *= fval; //X^2;
            tmp = fval * ft; //X^2 * N
            dev->var2[i] += tmp; //sigma( X^2*N )
        }
        last = start;
        start++;
    }

    t = t1 - t0;
    ft = CXDFSCALETIME( t ); //sigma(N(time) )
    for ( int i =0; i < 3; i++  ) {
        dev->mean[i] /= ft; //sigma(X*N) / sigma(N(time) )
        tmp = dev->mean[i] * dev->mean[i]; //Mean^2
        dev->var[i] = dev->var2[i];
        dev->var[i] -= tmp * ft; //sigma(X^2*N) - Mean^2*N
        dev->var[i] /= ft;
    }

    return nPoint;
}

void combineDevation( sDeviation3F *dst, sDeviation3F *d1, sDeviation3F *d2,
                                        float t, float t1, float t2 ) {
    float tmp = 0.f;
    for ( int i = 0; i < 3; i++ ) {
        dst->mean[i] = d1->mean[i] * t1 + d2->mean[i] * t2;
        dst->mean[i] /= t;
        dst->var2[i] = d1->var2[i] + d2->var2[i];
        dst->var[i] = dst->var2[i];
        tmp = dst->mean[i] * dst->mean[i];
        dst->var[i] -= tmp * t;
        dst->var[i] /= t;
    }
}

void CXSlide::combineSensorStage( cxSlideStage *dst, cxSlideStage *s1, cxSlideStage *s2 )
{
    float tmp = 0.f;
    dst->latency = s1->latency + s2->latency;
    dst->distance = s1->distance + s2->distance;
    dst->vel = dst->distance / dst->latency;
    dst->deviation.mean = s1->deviation.mean * s1->latency +s2->deviation.mean * s2->latency;
    dst->deviation.mean /= dst->latency;
    dst->deviation.var2 = s1->deviation.var2 + s2->deviation.var2; // sigma( X^2 *N)
    dst->deviation.var = dst->deviation.var2;
    tmp = dst->deviation.mean * dst->deviation.mean; //Mean^2
    dst->deviation.var -= tmp * dst->latency; //sigma( X^2 *N) - Mean^2*sigma(N)
    dst->deviation.var /= dst->latency;
    //pressure
    dst->pressure.mean = s1->pressure.mean * s1->latency + s2->pressure.mean * s2->latency;
    dst->pressure.mean /= dst->latency;
    dst->pressure.var2 = s1->pressure.var2 + s2->pressure.var2;
    dst->pressure.var =  dst->pressure.var2;
    tmp = dst->pressure.mean * dst->pressure.mean;
    dst->pressure.var -= tmp * dst->latency;
    dst->pressure.var /= dst->latency;

    combineDevation( &dst->grav, &s1->grav, &s2->grav, dst->latency, s1->latency, s2->latency );
    combineDevation( &dst->accs, &s1->accs, &s2->accs, dst->latency, s1->latency, s2->latency );
    combineDevation( &dst->gyro, &s1->gyro, &s2->gyro, dst->latency, s1->latency, s2->latency );

}

void CXSlide::normalize()
{
    //ceshi
//    m_feature.fval[0] = m_feature.fval[57] = 111;

    cxSlideStage *s = &m_stage[slide_touch];
    cxSlideStage *front = &m_stage[slide_front];
    cxSlideStage *back = &m_stage[slide_back];
    cxSlideStage *stage1 = &m_stage[slide_touch4];
    cxSlideStage *stage2 = &m_stage[slide_touch5];
    cxSlideStage *stage3 = &m_stage[slide_touch6];
    cxSlideStage *stage4 = &m_stage[slide_touch7];
    cxSlideStage *stage5 = &m_stage[slide_touch8];
    cxSlideStage *stage6 = &m_stage[slide_touch9];
    cxSlideStage *stage7 = &m_stage[slide_touch10];


    cxSlidePoint *start = &m_points[slide_start];
    cxSlidePoint *end = &m_points[slide_end];
    cxSlidePoint *ldp = &m_points[slide_ldp];
    cxSlidePoint *ldp1 = &m_points[slide_ldp4];
    cxSlidePoint *ldp2 = &m_points[slide_ldp5];
    cxSlidePoint *ldp3 = &m_points[slide_ldp6];
    cxSlidePoint *ldp4 = &m_points[slide_ldp7];
    cxSlidePoint *ldp5 = &m_points[slide_ldp8];
    cxSlidePoint *ldp6 = &m_points[slide_ldp9];
    cxSlidePoint *ldp7 = &m_points[slide_ldp10];
    cxSlidePoint *ldp8 = &m_points[slide_ldp11];



    m_feature.startX = start->touch.std.fx / m_screenSize; //position
    m_feature.startY = start->touch.std.fy / m_screenSize;
    m_feature.endX = end->touch.std.fx / m_screenSize;
    m_feature.endY = end->touch.std.fy / m_screenSize;
    m_feature.ldpX = ldp->touch.std.fx / m_screenSize;
    m_feature.ldpY = ldp->touch.std.fy / m_screenSize;
    m_feature.ldp1X = ldp1->touch.std.fx / m_screenSize;
    m_feature.ldp1Y = ldp1->touch.std.fy / m_screenSize;
    m_feature.ldp2X = ldp2->touch.std.fx / m_screenSize;
    m_feature.ldp2Y = ldp2->touch.std.fy / m_screenSize;
    m_feature.ldp3X = ldp3->touch.std.fx / m_screenSize;
    m_feature.ldp3Y = ldp3->touch.std.fy / m_screenSize;
    m_feature.ldp4X = ldp4->touch.std.fx / m_screenSize;
    m_feature.ldp4Y = ldp4->touch.std.fy / m_screenSize;
    m_feature.ldp5X = ldp5->touch.std.fx / m_screenSize;
    m_feature.ldp5Y = ldp5->touch.std.fy / m_screenSize;
    m_feature.ldp6X = ldp6->touch.std.fx / m_screenSize;
    m_feature.ldp6Y = ldp6->touch.std.fy / m_screenSize;
    m_feature.ldp7X = ldp7->touch.std.fx / m_screenSize;
    m_feature.ldp7Y = ldp7->touch.std.fy / m_screenSize;
    m_feature.ldp8X = ldp8->touch.std.fx / m_screenSize;
    m_feature.ldp8Y = ldp8->touch.std.fy / m_screenSize;

//    m_feature.gDirect = s->vector.direct; //direction
    m_feature.frontRotation = m_stage[slide_front].rotation / 16.f;
    m_feature.backRotation = m_stage[slide_back].rotation / 16.f;
    m_feature.ldp1Rotation = m_stage[slide_touch4].rotation / 16.f;
    m_feature.ldp2Rotation = m_stage[slide_touch5].rotation / 16.f;
    m_feature.ldp3Rotation = m_stage[slide_touch6].rotation / 16.f;
    m_feature.ldp4Rotation = m_stage[slide_touch7].rotation / 16.f;
    m_feature.ldp5Rotation = m_stage[slide_touch8].rotation / 16.f;
    m_feature.ldp6Rotation = m_stage[slide_touch9].rotation / 16.f;
    m_feature.ldp7Rotation = m_stage[slide_touch10].rotation / 16.f;

    m_feature.gRotation = rotation( start->touch.desc.direct, end->touch.desc.direct ) / 16.f;

    m_feature.gLength = s->vector.length / m_screenSize; //vector
//    m_feature.gDistance = s->distance / m_screenSize;
    m_feature.frontLength = front->vector.length / m_screenSize;
    m_feature.frontDistance = front->distance / m_screenSize;
    m_feature.backLength = back->vector.length / m_screenSize;
    m_feature.backDistance = back->distance / m_screenSize;
    m_feature.ldp1Length = stage1->vector.length / m_screenSize;
    m_feature.ldp1Distance = stage1->distance / m_screenSize;
    m_feature.ldp2Length = stage2->vector.length / m_screenSize;
    m_feature.ldp2Distance = stage2->distance / m_screenSize;
    m_feature.ldp3Length = stage3->vector.length / m_screenSize;
    m_feature.ldp3Distance = stage3->distance / m_screenSize;
    m_feature.ldp4Length = stage4->vector.length / m_screenSize;
    m_feature.ldp4Distance = stage4->distance / m_screenSize;
    m_feature.ldp5Length = stage5->vector.length / m_screenSize;
    m_feature.ldp5Distance = stage5->distance / m_screenSize;
    m_feature.ldp6Length = stage6->vector.length / m_screenSize;
    m_feature.ldp6Distance = stage6->distance / m_screenSize;
    m_feature.ldp7Length = stage7->vector.length / m_screenSize;
    m_feature.ldp7Distance = stage7->distance / m_screenSize;



    m_feature.gVelocty = s->vel / m_velocitySize; //vel
    m_feature.frontVelocity = front->vel / m_velocitySize;
    m_feature.backVelocity = back->vel / m_velocitySize;
    m_feature.ldp1Velocity = stage1->vel / m_velocitySize;
    m_feature.ldp2Velocity = stage2->vel / m_velocitySize;
    m_feature.ldp3Velocity = stage3->vel / m_velocitySize;
    m_feature.ldp4Velocity = stage4->vel / m_velocitySize;
    m_feature.ldp5Velocity = stage5->vel / m_velocitySize;
    m_feature.ldp6Velocity = stage6->vel / m_velocitySize;
    m_feature.ldp7Velocity = stage7->vel / m_velocitySize;


    m_feature.ldpDiviation = m_ldpDist[0] / m_screenSize; //div
    m_feature.ldp1Diviation = m_ldpDist[1] / m_screenSize; //div
    m_feature.ldp2Diviation = m_ldpDist[2] / m_screenSize; //div
    m_feature.ldp3Diviation = m_ldpDist[3] / m_screenSize; //div
    m_feature.ldp4Diviation = m_ldpDist[4] / m_screenSize; //div
    m_feature.ldp5Diviation = m_ldpDist[5] / m_screenSize; //div
    m_feature.ldp6Diviation = m_ldpDist[6] / m_screenSize; //div
    m_feature.ldp7Diviation = m_ldpDist[7] / m_screenSize; //div
    m_feature.ldp8Diviation = m_ldpDist[8] / m_screenSize; //div

    m_feature.gDivMean = s->deviation.mean / m_screenSize;
    m_feature.frontDivMean = front->deviation.mean / m_screenSize;
    m_feature.backDivMean = back->deviation.mean / m_screenSize;

    m_feature.gPressureMean = s->pressure.mean / m_pressureSize; //press mean
//    m_feature.frontPresMean = front->pressure.mean;
//    m_feature.backPresMean = back->pressure.mean;

    m_feature.gDivVar = s->deviation.var / m_screenSize; //div
    m_feature.frontDivVar = front->deviation.var / m_screenSize;
    m_feature.backDivVar = back->deviation.var / m_screenSize;
    m_feature.ldp1DivVar = stage1->deviation.var / m_screenSize;
    m_feature.ldp2DivVar = stage2->deviation.var / m_screenSize;
    m_feature.ldp3DivVar = stage3->deviation.var / m_screenSize;
    m_feature.ldp4DivVar = stage4->deviation.var / m_screenSize;
    m_feature.ldp5DivVar = stage5->deviation.var / m_screenSize;
    m_feature.ldp6DivVar = stage6->deviation.var / m_screenSize;
    m_feature.ldp7DivVar = stage7->deviation.var / m_screenSize;


    m_feature.startAcc = start->touch.desc.acc / m_accelerateSize; //acc
    m_feature.endAcc = end->touch.desc.acc / m_accelerateSize;
    m_feature.ldpAcc = ldp->touch.desc.acc / m_accelerateSize;
    m_feature.ldp1Acc = ldp1->touch.desc.acc / m_accelerateSize;
    m_feature.ldp2Acc = ldp2->touch.desc.acc / m_accelerateSize;
    m_feature.ldp3Acc = ldp3->touch.desc.acc / m_accelerateSize;
    m_feature.ldp4Acc = ldp4->touch.desc.acc / m_accelerateSize;
    m_feature.ldp5Acc = ldp5->touch.desc.acc / m_accelerateSize;
    m_feature.ldp6Acc = ldp6->touch.desc.acc / m_accelerateSize;
    m_feature.ldp7Acc = ldp7->touch.desc.acc / m_accelerateSize;
    m_feature.ldp8Acc = ldp8->touch.desc.acc / m_accelerateSize;
//    m_feature.gPressureVar = s->pressure.var / m_pressureSize; //press var

    m_feature.gGravMean[0] = s->grav.mean[0];//sensor global mean
    m_feature.gGravMean[1] = s->grav.mean[1];
    m_feature.gGravMean[2] = s->grav.mean[2];
//    m_feature.gAccsMean[0] = s->accs.mean[0];
//    m_feature.gAccsMean[1] = s->accs.mean[1];
//    m_feature.gAccsMean[2] = s->accs.mean[2];
    m_feature.gGyroMean[0] = s->gyro.mean[0];
    m_feature.gGyroMean[1] = s->gyro.mean[1];
    m_feature.gGyroMean[2] = s->gyro.mean[2];

//    m_feature.frontGyroMean[0] = front->gyro.mean[0];//gyro front, back mean
//    m_feature.frontGyroMean[1] = front->gyro.mean[1];
//    m_feature.frontGyroMean[2] = front->gyro.mean[2];
//    m_feature.backGyroMean[0] = back->gyro.mean[0];
//    m_feature.backGyroMean[1] = back->gyro.mean[1];
//    m_feature.backGyroMean[2] = back->gyro.mean[2];

    m_feature.gGravVar[0] = s->grav.var[0];//sensor global var
    m_feature.gGravVar[1] = s->grav.var[1];
    m_feature.gGravVar[2] = s->grav.var[2];
    m_feature.gAccsVar[0] = s->accs.var[0];
    m_feature.gAccsVar[1] = s->accs.var[1];
    m_feature.gAccsVar[2] = s->accs.var[2];
    m_feature.gGyroVar[0] = s->gyro.var[0];
    m_feature.gGyroVar[1] = s->gyro.var[1];
    m_feature.gGyroVar[2] = s->gyro.var[2];

//    m_feature.frontGyroVar[0] = front->gyro.var[0];//gyro front, back var
//    m_feature.frontGyroVar[1] = front->gyro.var[1];
//    m_feature.frontGyroVar[2] = front->gyro.var[2];
//    m_feature.backGyroVar[0] = back->gyro.var[0];
//    m_feature.backGyroVar[1] = back->gyro.var[1];
//    m_feature.backGyroVar[2] = back->gyro.var[2];

    //normalize gravity
    normalize3D( m_feature.gGravMean );
    normalize3D( m_feature.gGravVar );
}

void CXSlide::printPoint( FILE *fp, cxSlidePoint *pt )
{
    fprintf( fp, " %f %f %f \n", pt->touch.std.fx, pt->touch.std.fy, pt->touch.std.pressure ); //fx, fy, pressure
    fprintf( fp, " %f %f %d \n", pt->touch.desc.vel, pt->touch.desc.acc, pt->touch.desc.direct ); //velocity, accs, direct
    fprintf( fp, " %f %f %f \n", pt->grav[0].fx, pt->grav[0].fy, pt->grav[0].fz ); //grav
    fprintf( fp, " %f %f %f \n", pt->grav[1].fx, pt->grav[1].fy, pt->grav[1].fz ); //grav
    fprintf( fp, " %f %f %f \n", pt->accs[0].fx, pt->accs[0].fy, pt->accs[0].fz ); //accs
    fprintf( fp, " %f %f %f \n", pt->accs[1].fx, pt->accs[1].fy, pt->accs[1].fz ); //accs
    fprintf( fp, " %f %f %f \n", pt->gyro[0].fx, pt->gyro[0].fy, pt->gyro[0].fz ); //gyro
    fprintf( fp, " %f %f %f \n", pt->gyro[1].fx, pt->gyro[1].fy, pt->gyro[1].fz ); //gyro
}

void CXSlide::printStage( FILE *fp, cxSlideStage *stage )
{
    fprintf(  fp, " %f %f %f \n", stage->distance, stage->vel, stage->latency );
    fprintf(  fp, " %f %f \n", stage->deviation.mean, stage->deviation.var );
    fprintf(  fp, " %f %f \n", stage->pressure.mean, stage->pressure.var );
    fprintf(  fp, " %f %d %f \n", stage->vector.length, stage->vector.direct, stage->rotation );
    fprintf( fp, " %f %f %f \n", stage->grav.mean[0], stage->grav.mean[1], stage->grav.mean[2] ); //grav
    fprintf( fp, " %f %f %f \n", stage->grav.var[0], stage->grav.var[1], stage->grav.var[2] ); //grav
    fprintf( fp, " %f %f %f \n", stage->accs.mean[0], stage->accs.mean[1], stage->accs.mean[2] ); //accs
    fprintf( fp, " %f %f %f \n", stage->accs.var[0], stage->accs.var[1], stage->accs.var[2] ); //accs
    fprintf( fp, " %f %f %f \n", stage->gyro.mean[0], stage->gyro.mean[1], stage->gyro.mean[2] ); //gyro
    fprintf( fp, " %f %f %f \n", stage->gyro.var[0], stage->gyro.var[1], stage->gyro.var[2] ); //gyro
}

int CXSlide::readPoint(FILE *fp, cxSlidePoint *pt)
{
    if ( fscanf( fp, " %f %f %f " , &pt->touch.std.fx, &pt->touch.std.fy, &pt->touch.std.fz ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %d " , &pt->touch.desc.vel, &pt->touch.desc.acc, &pt->touch.desc.direct ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->grav[0].fx, &pt->grav[0].fy, &pt->grav[0].fz ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->grav[1].fx, &pt->grav[1].fy, &pt->grav[1].fz ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->accs[0].fx, &pt->accs[0].fy, &pt->accs[0].fz ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->accs[1].fx, &pt->accs[1].fy, &pt->accs[1].fz ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->gyro[0].fx, &pt->gyro[0].fy, &pt->gyro[0].fz ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->gyro[1].fx, &pt->gyro[1].fy, &pt->gyro[1].fz ) == EOF )
        return -1;

    return 0;
}

int CXSlide::readStage(FILE *fp, cxSlideStage *stage)
{
    if ( fscanf(  fp, " %f %f %f ", &stage->distance, &stage->vel, &stage->latency ) == EOF )
        return -1;
     if ( fscanf(  fp, " %f %f ", &stage->deviation.mean, &stage->deviation.var ) == EOF )
         return -1;
     if ( fscanf(  fp, " %f %f ", &stage->pressure.mean, &stage->pressure.var ) == EOF )
         return -1;
     if ( fscanf(  fp, " %f %d %f  ", &stage->vector.length, &stage->vector.direct, &stage->rotation ) == EOF )
         return -1;
     if ( fscanf( fp, " %f %f %f ", &stage->grav.mean[0], &stage->grav.mean[1], &stage->grav.mean[2] ) == EOF ) //grav
         return -1;
     if ( fscanf( fp, " %f %f %f ", &stage->grav.var[0], &stage->grav.var[1], &stage->grav.var[2] ) == EOF ) //grav
         return -1;
     if ( fscanf( fp, " %f %f %f ", &stage->accs.mean[0], &stage->accs.mean[1], &stage->accs.mean[2] ) == EOF ) //accs
         return -1;
     if ( fscanf( fp, " %f %f %f ", &stage->accs.var[0], &stage->accs.var[1], &stage->accs.var[2] ) == EOF ) //accs
         return -1;
     if ( fscanf( fp, " %f %f %f ", &stage->gyro.mean[0], &stage->gyro.mean[1], &stage->gyro.mean[2] ) == EOF ) //gyro
         return -1;
     if ( fscanf( fp, " %f %f %f ", &stage->gyro.var[0], &stage->gyro.var[1], &stage->gyro.var[2] ) == EOF ) //gyro
         return -1;

    return 0;
}


