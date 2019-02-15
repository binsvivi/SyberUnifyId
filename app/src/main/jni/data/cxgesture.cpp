#include "cxslide.h"
#include "cxgesture.h"
#include "cxmath.h"
#include "cxsensordata.h"

float CXGesture::m_screenSize = 1920.f;
float CXGesture::m_pressureSize = 0.5f;
float CXGesture::m_velocitySize = 192000.f;
float CXGesture::m_accelerateSize = 1920000.f;

double gesture_feature_scale[GESTURE_FEATURE_DIM_SIZE] = {
  1.,//  GESTURE_END_X = 0, //POSITION               位置,表达gesture在space的位置区域
  1.,// GESTURE_END_Y,
  1.,//  GESTURE_END_Z,
  1.,//  GESTURE_LDP_X,
  1.,//  GESTURE_LDP_Y,
  1.,//  GESTURE_LDP_Z,
  1.,//  GESTURE_FACE_START_X, // FACE POSITION               位置,表达gesture在space的位置区域
  1.,//  GESTURE_FACE_START_Y,
  1.,//  GESTURE_FACE_START_Z,
  1. ,//  GESTURE_FACE_END_X,
   1. ,// GESTURE_FACE_END_Y,
  1. ,//  GESTURE_FACE_END_Z,
  1. ,//  GESTURE_FACE_LDP_X,
  1. ,//  GESTURE_FACE_LDP_Y,
  1. ,//  GESTURE_FACE_LDP_Z,
  1. ,//  GESTURE_GYRO_START_X, // GYRO VELOCITY               位置,表达gesture在space的位置区域
  1. ,//  GESTURE_GYRO_START_Y,
  1. ,//  GESTURE_GYRO_START_Z,
  1. ,//  GESTURE_GYRO_END_X,
  1. ,//  GESTURE_GYRO_END_Y,
  1. ,//  GESTURE_GYRO_END_Z,
  1. ,//  GESTURE_GYRO_LDP_X,
  1. ,//  GESTURE_GYRO_LDP_Y,
  1. ,//  GESTURE_GYRO_LDP_Z,
  1. ,//  GESTURE_FACE_DIR_ROT_FULL_X, // GYRO VELOCITY               位置,表达gesture在space的位置区域
  1. ,//  GESTURE_FACE_DIR_ROT_FULL_Y,
  1. ,//  GESTURE_FACE_DIR_ROT_FULL_Z,
  1. ,//  GESTURE_FACE_DIR_ROT_FRONT_X,
  1. ,//  GESTURE_FACE_DIR_ROT_FRONT_Y,
  1. ,//  GESTURE_FACE_DIR_ROT_FRONT_Z,
  1. ,//  GESTURE_FACE_DIR_ROT_BACK_X,
  1. ,//  GESTURE_FACE_DIR_ROT_BACK_Y,
  1. ,//  GESTURE_FACE_DIR_ROT_BACK_Z,
  1. ,//  GESTURE_GLOBAL_DISTANCE, //                                 轨迹长度, 矢量长度                  4
  1. ,//  GESTURE_GLOBAL_LENGTH, //                                 轨迹长度, 矢量长度                  4
  1. ,//  GESTURE_FRONT_DISTANCE, //                                 轨迹长度, 矢量长度                  4
  1. ,//  GESTURE_FRONT_LENGTH, //                                 轨迹长度, 矢量长度                  4
  1. ,//  GESTURE_BACK_DISTANCE, //                                 轨迹长度, 矢量长度                  4
  1. ,//  GESTURE_GLOBAL_VEL, //VELOCITY                    速度分布                                        5
  1. ,//  GESTURE_FRONT_VEL,
  1. ,//  GESTURE_BACK_VEL,
  1. ,//  GESTURE_LDP_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
  1. ,//  GESTURE_GLOBAL_DIV_MEAN, //VELOCITY                    速度分布                                        5
  1. ,//  GESTURE_FRONT_DIV_MEAN,
  1. ,//  GESTURE_BACK_DIV_MEAN,
  1. ,//  GESTURE_GLOBAL_DIV_VAR, //VELOCITY                    速度分布                                        5
  1. ,//  GESTURE_FRONT_DIV_VAR,
  1. ,//  GESTURE_BACK_DIV_VAR,
  1. ,//  GESTURE_GACC_START_X, // GYRO VELOCITY               位置,表达gesture在space的位置区域
  1. ,//  GESTURE_GACC_START_Y,
  1. ,//  GESTURE_GACC_START_Z,
  1. ,//  GESTURE_GACC_END_X,
  1. ,//  GESTURE_GACC_END_Y,
  1. ,//  GESTURE_GACC_END_Z,
  1. ,//  GESTURE_GACC_LDP_X,
  1. ,//  GESTURE_GACC_LDP_Y,
  1. ,//  GESTURE_GACC_LDP_Z,


};

void featGestureSample( CXGesture *gesture, sensor_sample *sample, const char *featPath ) {
    gesture->loadSample( sample );

    int pathLength = strlen(featPath);
    char buffer[1024] = {0};
    memset( buffer, 0, 1024 );
    memcpy( buffer, featPath, pathLength );
    char *ptr = buffer + pathLength;
    char *fpathStr = sample->key.getKeyStr() ;
    int keyLen = strlen( fpathStr );
    memcpy( ptr, fpathStr, keyLen );
    ptr += keyLen;
    const char *gestureExt = ".gesture";
    const char *featExt = ".feat";
    int glSize = strlen( gestureExt );
    int ftSize = strlen( featExt );
    memcpy( ptr, featExt, ftSize );
    gesture->saveFeat( buffer );
    memset( ptr, 0,ftSize );
    memcpy( ptr, gestureExt, glSize );
    gesture->save( buffer );
}

void  featGestures( CXSensorSample *sampler, const char* samplePath, const char *featPath ) {

    sampler->readSensor( samplePath );
    fprintf( stderr, "read sample ~~~~~~~~~~");

    CXGesture gesture;
    for ( int i = 0; i < sampler->m_task.nSample; i++ ) {
        sensor_sample *sample = sampler->m_task.samples[i];
      //  featSlideSample( &slide, sample, featPath );
        featGestureSample( &gesture, sample, featPath );
    }
    CXSensorSample::gSampleMap.clear();
//    CXSensorSample::gSampleBuffer.clear();
}

void  ceshiGesture() {
    const char* samplePath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/input/samples-gestures.info";

    const char *featPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/feature/gesture/train/shabin/";

    const char* shabinPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/input/samples-gestures-test.info";
    const char* xizhiPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/input/samples-gestures-test-xizhi.info";
    const char* sunquanyuPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/input/samples-gestures-test-xizhi.info";
    const char* liangyangangPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/input/samples-gestures-test-xizhi.info";


    const char *shabinfeatPath = "/home/binsha/work/ai/unify-id-data/UnifyID/feature/gesture/test/shabin/";
    const char *xizhifeatPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/feature/gesture/test/xizhi/";
    const char *sunquanyufeatPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/feature/gesture/test/sunquanyu/";
    const char *liangyangangfeatPath = "/home/binsha/work/SyberAI/unify-id-data/UnifyID/feature/gesture/test/liangyangang/";


    CXSensorSample sampler;

    //featSlides( &sampler, samplePath, featPath );
   // featGestures( &sampler, samplePath, featPath );

   // sampler.clear();

    featGestures( &sampler, xizhiPath, xizhifeatPath );
    sampler.clear();

}

CXGesture::CXGesture()
{
}

CXGesture::~CXGesture()
{}

void CXGesture::clear()
{}

int CXGesture::loadSample( sensor_sample *sample )
{
    sdata_packet *paccs = &sample->sensors[SENSOR_ACCS];
    if ( paccs->isEmpty() ) {
        fprintf( stderr, "sensor samples is empty!" );
        return -1;
    }

    sensor_data *sdata  = &paccs->sdata;
    if ( sdata->nData < 3 ) {
        fprintf( stderr, "not have enough sensor samples, number of sample must > 3 !" );
        return -1;
    }

    //accsSubGrav( sample );

  //  cxSlidePoint *pt = m_points;
    cxGesturePoint *pt = m_points;
    int pos = sdata->nData-1;

    cxPoint3F start;
    cxPoint3F end;
    cxPoint3F ldpPt;
   // start.fx = sdata->tdatas[0].val[0];
   // start.fy = sdata->tdatas[0].val[1];
   // start.fz = sdata->tdatas[0].val[2];
    start.fx = 0;
    start.fy = 0;
    start.fz = 0;


    end.fx = sdata->tdatas[pos].val[0];
    end.fy = sdata->tdatas[pos].val[1];
    end.fz = sdata->tdatas[pos].val[2];
    m_location = (cxPoint3F*)malloc(sizeof(cxPoint3F)*pos);

    sdata_packet *sgrav = sample->sensors + 1;
  //  sdata_packet *saccs = sample->sensors + 2;
    sdata_packet *sgyro = sample->sensors + 3;
    sdata_packet *smagn = sample->sensors + 4;
    sensor_data *accs  = &paccs->sdata;
    sensor_data *grav  = &sgrav->sdata;
    sensor_data *gyro  = &sgyro->sdata;
    sensor_data *magn  = &smagn->sdata;

    int limit = accs->nData - 1;

    int i = limit - 1;
    int j = 0;
    float vx = 0;
    float vy = 0;
    float vz = 0;
    float sx = 0;
    float sy = 0;
    float sz = 0;

    while (j <= i) {
        float t0 = accs->tdatas[j].local_time;
        float t1 = accs->tdatas[j+1].local_time;
        float magf0[3] = {0.f};
        float gval0[3] = {0.f};
        float magf1[3] = {0.f};
        float gval1[3] = {0.f};

        float accf0[3] = {0.f};
        float accf1[3] = {0.f};

        interSensorPoint(magn, t0, magf0[0], magf0[1], magf0[2]);
        interSensorPoint(grav, t0, gval0[0], gval0[1], gval0[2]);
    //    fprintf( stderr, "mag 0 is %d %f %f %f\n", j, magf0[0], magf0[1], magf0[2]);
    //    fprintf( stderr, "grav 0 is %d %f %f %f\n", j, gval0[0], gval0[1], gval0[2]);

        interSensorPoint(magn, t1, magf1[0], magf1[1], magf1[2]);
        interSensorPoint(grav, t1, gval1[0], gval1[1], gval1[2]);

        calculateOrientation(accs, j, gval0, magf0, accf0);
        calculateOrientation(accs, j+1, gval1, magf1, accf1);

        //float ax = 0.5*(accs->tdatas[j+1].val[0] + accs->tdatas[j].val[0]);
       // float ay = 0.5*(accs->tdatas[j+1].val[1] + accs->tdatas[j].val[1]);
       // float az = 0.5*(accs->tdatas[j+1].val[2] + accs->tdatas[j].val[2]);
        float ax = 0.5*(accf0[0] + accf1[0]);
        float ay = 0.5*(accf0[1] + accf1[1]);
        float az = 0.5*(accf0[2] + accf1[2]);
       // fprintf( stderr, "a is %d %f %f %f\n", j, ax, ay, az);

        float t = accs->tdatas[j+1].local_time - accs->tdatas[j].local_time;
        float ft = CXDFSCALETIME(t);
        sx = sx + vx * ft + 0.5*(ax * ft *ft);
        sy = sy + vy * ft + 0.5*(ay * ft *ft);
        sz = sz + vz * ft + 0.5*(az * ft *ft);
        m_location[j].fx = sx;
        m_location[j].fy = sy;
        m_location[j].fz = sz;
       fprintf( stderr, "m_location is %d %f %f %f\n", j, sx, sy, sz);
        vx = vx + ax * ft;
        vy = vy + ay * ft;
        vz = vz + az * ft;
         //  fprintf( stderr, "vel is %d %f %f %f\n", j, vx, vy, vz);

        j++;
    }



    //slidePoint( pt++, sample, 0 );
    gesturePoint( pt++, sample, 0 );
   // slidePoint( pt++, sample, pos );
    gesturePoint( pt++, sample, pos );
    end.fx = m_points[gesture_end].location.fx;
    end.fy = m_points[gesture_end].location.fy;
    end.fz = m_points[gesture_end].location.fz;


    cxLine3dDesc line;
    int ldp = 0;
    float *dist = (float*)malloc( sizeof(float) * sdata->nData );
    /*
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
*/
    line.m = end.fx - start.fx;
    line.n = end.fy - start.fy;
    line.p = end.fz - start.fz;
    line.fx = start.fx;
    line.fy = start.fy;
    line.fz = start.fz;

    ldp = findLdpPosition( sdata, dist, line );
    m_sDesc.gesture.ldpDistance = dist[ldp];
    m_ldpDist = dist[ldp];
    fprintf( stderr, "ldp is %d %f \n",ldp,  m_ldpDist);

    ldpPt.fx = sdata->tdatas[ldp].val[0];
    ldpPt.fy = sdata->tdatas[ldp].val[1];
    ldpPt.fz = sdata->tdatas[ldp].val[2];
//    slidePoint( pt, sample, ldp );
    gesturePoint( pt, sample, ldp );


    cxGestureStage *front = m_stage;
  //  front->rotation = rotation( m_points[slide_start].touch.desc.direct,
    //                                   m_points[slide_ldp].touch.desc.direct );
    front->rotationX = rotationf( m_points[gesture_start].position.std.fx,
                                       m_points[gesture_ldp].position.std.fx );
    front->rotationY = rotationf( m_points[gesture_start].position.std.fy,
                                       m_points[gesture_ldp].position.std.fy );
    front->rotationZ = rotationf( m_points[gesture_start].position.std.fz,
                                       m_points[gesture_ldp].position.std.fz );
    gestureStage( front, sample, dist, 0, ldp );

    cxGestureStage *back = front + 1;
  //  back->rotation = rotation( m_points[slide_ldp].touch.desc.direct,
 //                                      m_points[slide_end].touch.desc.direct );
    back->rotationX = rotationf( m_points[gesture_ldp].position.std.fx,
                                       m_points[gesture_end].position.std.fx );
    back->rotationY = rotationf( m_points[gesture_ldp].position.std.fy,
                                       m_points[gesture_end].position.std.fy );
    back->rotationZ = rotationf( m_points[gesture_ldp].position.std.fz,
                                       m_points[gesture_end].position.std.fz );
    gestureStage( back, sample, dist, ldp, pos );

    cxGestureStage *stage = back + 1;
  //  stage->rotation = rotation( m_points[slide_start].touch.desc.direct,
   //                                                                         m_points[slide_end].touch.desc.direct );
    stage->rotationX = rotationf( m_points[gesture_start].position.std.fx,
                                       m_points[gesture_end].position.std.fx );
    stage->rotationY = rotationf( m_points[gesture_start].position.std.fy,
                                       m_points[gesture_end].position.std.fy );
    stage->rotationZ = rotationf( m_points[gesture_start].position.std.fz,
                                       m_points[gesture_end].position.std.fz );
//    slideStage( stage, sample, dist, 0, pos );
    vector( stage->vector, start, end );
    combineSensorStage( stage, front, back);

    normalize();
    CXDFFREE( dist );

    return 0;
}


void CXGesture::save( const char *filepath )
{
    FILE *fp = fopen( filepath, "w" );
    if ( !fp )
        return;
    cxGesturePoint *pt = m_points;
    fprintf( fp, "<start>\n" );
    printPoint( fp, pt );
    pt++;
    fprintf( fp, "<end>\n" );
    printPoint( fp, pt );
    pt++;
    fprintf( fp, "<ldp>\n" );
    printPoint( fp, pt );

    cxGestureStage *stage = m_stage;
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



void CXGesture::saveFeat( const char *filepath )
{
    FILE *fp = fopen( filepath, "w" );
    if ( !fp )
        return;

    for ( int i = 0; i < GESTURE_FEATURE_DIM_SIZE; i++ ) {
       //  if (i == gestureFeatureSize - 1)
            fprintf( fp, "%f\n", m_feature.fval[i] );
      //   else
      //     fprintf( fp, "%f,", m_feature.fval[i] );

    }

    fclose( fp );
}

int CXGesture::loadFile( const char *filepath )
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
            ret = readPoint( fp, &m_points[gesture_start] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, end, 5 ) ) {
            ret = readPoint( fp, &m_points[gesture_end] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, ldp, 5 ) ) {
            ret = readPoint( fp, &m_points[gesture_ldp] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, stage, 7 ) ) {
            ret = readStage( fp, &m_stage[gesture_full] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, front, 7 ) ) {
            ret = readStage( fp, &m_stage[gesture_front] );
            if ( ret )
                break;
        } else if ( !strncmp( buffer, back, 6 ) ) {
            ret = readStage( fp, &m_stage[gesture_back] );
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

int CXGesture::loadFeature( const char *filepath )
{
    FILE *fp = fopen( filepath, "r" );
    if ( !fp )
        return -1;
    int i = 0;
    while( fscanf( fp, "%f" , &m_feature.fval[i] ) != EOF ) {
        i++;
    }

    fclose( fp );

    if ( i != GESTURE_FEATURE_DIM_SIZE )
        return -1;

    return 0;
}

/*
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
*/
int CXGesture::findLdpPosition( sensor_data *sample, float *distances, cxLine3dDesc &line )
{
    int i = 2;
    int limit = sample->nData - 1;
    int ldp = 1;
    float dist = 0.;
    float tmp = 0.;
    sensor_TData *td = sample->tdatas;

    dist = distance( m_location[0].fx, m_location[0].fy, m_location[0].fz, line );
    distances[0] = 0.;
    distances[ldp] = dist;
    distances[limit] = 0.;
    while ( i < limit ) {
        tmp = distance( m_location[i-1].fx, m_location[i-1].fy, m_location[i-1].fz, line );
        fprintf( stderr, "distance is %i %f\n",i, tmp);
        distances[i] = tmp;
        if ( tmp > dist ) {
            ldp = i;
            dist = tmp;
        }
        i++;
    }

    return ldp;
}

bool CXGesture::getRotationMatrix(float * R, float * I, float * gravity, float * geomagnetic)
{

        float Ax = gravity[0];
        float Ay = gravity[1];
        float Az = gravity[2];

        float normsqA = (Ax*Ax + Ay*Ay + Az*Az);
        float g = 9.81f;
        float freeFallGravitySquared = 0.01f * g * g;
        if (normsqA < freeFallGravitySquared) {
            // gravity less than 10% of normal value
            return false;
        }

        float Ex = geomagnetic[0];
        float Ey = geomagnetic[1];
        float Ez = geomagnetic[2];

        float Hx = Ey*Az - Ez*Ay;
        float Hy = Ez*Ax - Ex*Az;
        float Hz = Ex*Ay - Ey*Ax;
        float normH = (float)sqrt(Hx*Hx + Hy*Hy + Hz*Hz);

        if (normH < 0.1f) {
            // device is close to free fall (or in space?), or close to
            // magnetic north pole. Typical values are  > 100.
            return false;
        }

        float invH = 1.0f / normH;
        Hx *= invH;
        Hy *= invH;
        Hz *= invH;
        float invA = 1.0f / (float)sqrt(Ax*Ax + Ay*Ay + Az*Az);
        Ax *= invA;
        Ay *= invA;
        Az *= invA;
        float Mx = Ay*Hz - Az*Hy;
        float My = Az*Hx - Ax*Hz;
        float Mz = Ax*Hy - Ay*Hx;
        if (R != NULL) {
        //    if (R.length == 9) {
                R[0] = Hx;     R[1] = Hy;     R[2] = Hz;
                R[3] = Mx;     R[4] = My;     R[5] = Mz;
                R[6] = Ax;     R[7] = Ay;     R[8] = Az;
         //   } else if (R.length == 16) {
         //       R[0]  = Hx;    R[1]  = Hy;    R[2]  = Hz;   R[3]  = 0;
           //     R[4]  = Mx;    R[5]  = My;    R[6]  = Mz;   R[7]  = 0;
           //     R[8]  = Ax;    R[9]  = Ay;    R[10] = Az;   R[11] = 0;
           //     R[12] = 0;     R[13] = 0;     R[14] = 0;    R[15] = 1;
           // }
        }
        if (I != NULL) {
            // compute the inclination matrix by projecting the geomagnetic
            // vector onto the Z (gravity) and X (horizontal component
            // of geomagnetic vector) axes.
            float invE = 1.0f / (float)sqrt(Ex*Ex + Ey*Ey + Ez*Ez);
            float c = (Ex*Mx + Ey*My + Ez*Mz) * invE;
            float s = (Ex*Ax + Ey*Ay + Ez*Az) * invE;
          //  if (I.length == 9) {
                I[0] = 1;     I[1] = 0;     I[2] = 0;
                I[3] = 0;     I[4] = c;     I[5] = s;
                I[6] = 0;     I[7] =-s;     I[8] = c;
      //      } else if (I.length == 16) {
          //      I[0] = 1;     I[1] = 0;     I[2] = 0;
        //        I[4] = 0;     I[5] = c;     I[6] = s;
         //       I[8] = 0;     I[9] =-s;     I[10]= c;
        //        I[3] = I[7] = I[11] = I[12] = I[13] = I[14] = 0;
        //        I[15] = 1;
        //    }
        }
        return true;

}

void CXGesture::calculateOrientation(sensor_data * accs_data, int index, float * gvalues, float * magneticfieldvalues, float * tmp){
    float accs[3] = {accs_data->tdatas[index].val[0], accs_data->tdatas[index].val[1], accs_data->tdatas[index].val[2]};
    float rotate[9] = {0.f};
    getRotationMatrix(rotate, NULL, gvalues, magneticfieldvalues);
    tmp[0] = rotate[0] * accs[0] + rotate[1]*accs[1] + rotate[2]*accs[2];
    tmp[1] = rotate[3] * accs[0] + rotate[4]*accs[1] + rotate[5]*accs[2];
    tmp[2] = rotate[6] * accs[0] + rotate[7]*accs[1] + rotate[8]*accs[2];
   // fprintf( stderr, " calculateOrientationis %f %f %f\n", tmp[0], tmp[1], tmp[2]);

}

void CXGesture::getOrientation(float * R, float * values)
{
    values[0] = (float) atan2(R[1], R[4]);
    values[1] = (float) asin(-R[7]);
    values[2] = (float) atan2(-R[6], R[8]);
}

void CXGesture::gesturePoint( cxGesturePoint *pt, sensor_sample *sample, int index )
{
    gestureSensorPoint( pt, sample, index );
   // slideSensorPoint( pt, sample, index );
}

void CXGesture::gestureSensorPoint( cxGesturePoint *pt, sensor_sample *sample, int index )
{
    sdata_packet *sgrav = sample->sensors + 1;
    sdata_packet *saccs = sample->sensors + 2;
    sdata_packet *sgyro = sample->sensors + 3;
    sdata_packet *smagn = sample->sensors + 4;
    sensor_data *accs  = &saccs->sdata;
    sensor_data *grav  = &sgrav->sdata;
    sensor_data *gyro  = &sgyro->sdata;
    sensor_data *magn  = &smagn->sdata;

    int limit = accs->nData - 1;
    float fx = 0.;
    float fy = 0.;
    int i = 0;
    int j = 0;
    int64_t t = 0;
    float ft = 0.f;
    float dist = 0.f;
    float val = 0.f;
    float tmp = 0.f;
    int n = 0;
    float gx = 0.f;
    float gy = 0.f;
    float gz = 0.f;
    float px = 0.f;
    float py = 0.f;
    float pz = 0.f;
    float valx = 0.f;
    float valy = 0.f;
    float valz = 0.f;
    float tmpx = 0.f;
    float tmpy = 0.f;
    float tmpz = 0.f;
    int64_t tmpt = 0.f;
    t = sample->sensors[SENSOR_ACCS].sdata.tdatas[index].local_time;
    interSensorPoint(grav, t, gx, gy, gz);
    // need to calculate the initial position using G-sensor
    float ax = acosf(gx/9.8);
    float ay = acosf(gy/9.8);
    float az = acosf(gz/9.8);

    //pt->position.std.fx = getTanAngle(tanf(ax));                //x
  //  pt->position.std.fy = getTanAngle(tanf(ay));                //y
  //  pt->position.std.fz = getTanAngle(tanf(az));    //z
    pt->position.std.fx = ax;                //x
    pt->position.std.fy = ay;                //y
    pt->position.std.fz = az;    //z
/*
    i = limit - 1;
    j = 0;
    float vx = 0;
    float vy = 0;
    float vz = 0;
    float sx = 0;
    float sy = 0;
    float sz = 0;

    while (j <= i) {
        float t0 = accs->tdatas[j].local_time;
        float t1 = accs->tdatas[j+1].local_time;
        float magf0[3] = {0.f};
        float gval0[3] = {0.f};
        float magf1[3] = {0.f};
        float gval1[3] = {0.f};

        float accf0[3] = {0.f};
        float accf1[3] = {0.f};

        interSensorPoint(magn, t0, magf0[0], magf0[1], magf0[2]);
        interSensorPoint(grav, t0, gval0[0], gval0[1], gval0[2]);

        interSensorPoint(magn, t1, magf1[0], magf1[1], magf1[2]);
        interSensorPoint(grav, t1, gval1[0], gval1[1], gval1[2]);

        calculateOrientation(accs, j, gval0, magf0, accf0);
        calculateOrientation(accs, j+1, gval1, magf1, accf1);

        //float ax = 0.5*(accs->tdatas[j+1].val[0] + accs->tdatas[j].val[0]);
       // float ay = 0.5*(accs->tdatas[j+1].val[1] + accs->tdatas[j].val[1]);
       // float az = 0.5*(accs->tdatas[j+1].val[2] + accs->tdatas[j].val[2]);
        float ax = 0.5*(accf0[0] + accf1[0]);
        float ay = 0.5*(accf0[1] + accf1[1]);
        float az = 0.5*(accf0[2] + accf1[2]);

        t = accs->tdatas[j+1].local_time - accs->tdatas[j].local_time;
        float ft = CXDFSCALETIME(t);
        sx = sx + vx * ft + 0.5*(ax * ft *ft);
        sy = sy + vy * ft + 0.5*(ay * ft *ft);
        sz = sz + vz * ft + 0.5*(az * ft *ft);
        m_location[j].fx = sx;
        m_location[j].fy = sy;
        m_location[j].fz = sz;
        fprintf( stderr, "m_location is %d %f %f %f\n", j, sx, sy, sz);
        vx = vx + ax * ft;
        vy = vy + ay * ft;
        vz = vz + az * ft;

        j++;
    }
*/
   // getPointShift( touch, index, fx, fy, dist, t );
    getPointShift(accs, index, px, py, pz, tmpt);


    float gyrox = 0;
    float gyroy = 0;
    float gyroz = 0;
    int gyro_index = interSensorPoint(gyro, t, gyrox, gyroy, gyroz);


    float magf0[3] = {0.f};
    float gval0[3] = {0.f};
    float gyrof0[3] = {0.f};

    interSensorPoint(magn, t, magf0[0], magf0[1], magf0[2]);
    interSensorPoint(grav, t, gval0[0], gval0[1], gval0[2]);
//    fprintf( stderr, "mag 0 is %d %f %f %f\n", j, magf0[0], magf0[1], magf0[2]);
//    fprintf( stderr, "grav 0 is %d %f %f %f\n", j, gval0[0], gval0[1], gval0[2]);

    calculateOrientation(gyro, gyro_index, gval0, magf0, gyrof0);



    pt->gyro[0].fx = gyrox;
    pt->gyro[0].fy = gyroy;
    pt->gyro[0].fz = gyroz;
    pt->location.fx = px;
    pt->location.fy = py;
    pt->location.fz = pz;

  //  pt->touch.desc.direct = getTAngle( fx, fy );
    ft = CXDFSCALETIME( t );
    //pt->touch.desc.vel = dist / ft;
    if ( !gyro_index ) {
       // getPointShift( touch, 1, fx, fy, dist, t );
   //     getPointShift(accs, 1, px, py, pz, t);
        int64_t t1 = sample->sensors[SENSOR_GYRO].sdata.tdatas[1].local_time;
        int gyro_index1 = interSensorPoint(gyro, t1, gyrox, gyroy, gyroz);
        float magf1[3] = {0.f};
        float gval1[3] = {0.f};
        float gyrof1[3] = {0.f};

        interSensorPoint(magn, t1, magf1[0], magf1[1], magf1[2]);
        interSensorPoint(grav, t1, gval1[0], gval1[1], gval1[2]);
    //    fprintf( stderr, "mag 0 is %d %f %f %f\n", j, magf0[0], magf0[1], magf0[2]);
    //    fprintf( stderr, "grav 0 is %d %f %f %f\n", j, gval0[0], gval0[1], gval0[2]);

        calculateOrientation(gyro, gyro_index1, gval1, magf1, gyrof1);

       // gyrox = gyro->tdatas[1].val[0];
        //gyroy = gyro->tdatas[1].val[1];
       // gyroz = gyro->tdatas[1].val[2];
        ft = CXDFSCALETIME( t );
        valx = gyrof1[0] - pt->gyro[0].fx;
        valy = gyrof1[1] - pt->gyro[0].fy;
        valz = gyrof1[2] - pt->gyro[0].fz;
        //val -= pt->touch.desc.vel;
        t = t1 - t;
    } else if ( gyro_index == (sample->sensors[SENSOR_GYRO].sdata.nData - 1) ) {
        i =  (sample->sensors[SENSOR_GYRO].sdata.nData - 1)- 1;
        int64_t t1 = sample->sensors[SENSOR_GYRO].sdata.tdatas[i].local_time;
        //getPointShift( touch, i, fx, fy, dist, t );
   //     getPointShift(accs, i, px, py, pz, t);
        ft = CXDFSCALETIME( t );
        int gyro_index1 = interSensorPoint(gyro, t1, gyrox, gyroy, gyroz);
        float magf1[3] = {0.f};
        float gval1[3] = {0.f};
        float gyrof1[3] = {0.f};

        interSensorPoint(magn, t1, magf1[0], magf1[1], magf1[2]);
        interSensorPoint(grav, t1, gval1[0], gval1[1], gval1[2]);
    //    fprintf( stderr, "mag 0 is %d %f %f %f\n", j, magf0[0], magf0[1], magf0[2]);
    //    fprintf( stderr, "grav 0 is %d %f %f %f\n", j, gval0[0], gval0[1], gval0[2]);

        calculateOrientation(gyro, gyro_index1, gval1, magf1, gyrof1);
        //gyrox = gyro->tdatas[i].val[0];
       // gyroy = gyro->tdatas[i].val[1];
        //gyroz = gyro->tdatas[i].val[2];
        valx = pt->gyro[0].fx - gyrof1[0];
        valy = pt->gyro[0].fy - gyrof1[1];
        valz = pt->gyro[0].fz - gyrof1[2];
       // val = dist / ft;
       // val = pt->touch.desc.vel - val;
        t = t - t1;
    } else {
        i = gyro_index - 1;
        j = gyro_index + 1;

        int64_t t1 = sample->sensors[SENSOR_GYRO].sdata.tdatas[j].local_time;
        int gyro_index1 = interSensorPoint(gyro, t1, gyrox, gyroy, gyroz);
        float magf1[3] = {0.f};
        float gval1[3] = {0.f};
        float gyrof1[3] = {0.f};

        interSensorPoint(magn, t1, magf1[0], magf1[1], magf1[2]);
        interSensorPoint(grav, t1, gval1[0], gval1[1], gval1[2]);
    //    fprintf( stderr, "mag 0 is %d %f %f %f\n", j, magf0[0], magf0[1], magf0[2]);
    //    fprintf( stderr, "grav 0 is %d %f %f %f\n", j, gval0[0], gval0[1], gval0[2]);

        calculateOrientation(gyro, gyro_index1, gval1, magf1, gyrof1);
        valx = gyrof1[0];
        valy = gyrof1[1];
        valz = gyrof1[2];
       // getPointShift( touch, i, fx, fy, dist, t );
   //     getPointShift(accs, i, px, py, pz, t);
        ft = CXDFSCALETIME( t );
        int64_t t2 = sample->sensors[SENSOR_GYRO].sdata.tdatas[i].local_time;
        gyro_index1 = interSensorPoint(gyro, t2, gyrox, gyroy, gyroz);
        magf1[0] = 0.f;
        gval1[0] = 0.f;
        gyrof1[0] = 0.f;

        magf1[1] = 0.f;
        gval1[1] = 0.f;
        gyrof1[1] = 0.f;

        magf1[2] = 0.f;
        gval1[2] = 0.f;
        gyrof1[2] = 0.f;


        interSensorPoint(magn, t1, magf1[0], magf1[1], magf1[2]);
        interSensorPoint(grav, t1, gval1[0], gval1[1], gval1[2]);
    //    fprintf( stderr, "mag 0 is %d %f %f %f\n", j, magf0[0], magf0[1], magf0[2]);
    //    fprintf( stderr, "grav 0 is %d %f %f %f\n", j, gval0[0], gval0[1], gval0[2]);

        calculateOrientation(gyro, gyro_index1, gval1, magf1, gyrof1);
        tmpx = gyrof1[0];
        tmpy = gyrof1[1];
        tmpz = gyrof1[2];
        valx -= tmpx;
        valy -= tmpy;
        valz -= tmpz;
        t = t1 - t2;
    }
    ft = CXDFSCALETIME( t );
    //val /= ft;
    valx /= ft;
    valy /= ft;
    valz /= ft;
    //pt->touch.desc.acc = val;
    pt->position.desc.accX = valx;
    pt->position.desc.accY = valy;
    pt->position.desc.accZ = valz;
    return;

}

int CXGesture::interSensorPoint(sensor_data *sample, int64_t t , float &fx, float &fy, float &fz)
{
    int limit = sample->nData - 1;
    int n = 0;
    int i = 0;
    int j = 0;
    float dt = 0.f;
    while ( n < limit ) {
        if ( sample->tdatas[n].local_time >= t )
            break;
        n++;
    }
    if ( !n || n ==limit || sample->tdatas[n].local_time == t ) {
        fx = sample->tdatas[n].val[0];
        fy = sample->tdatas[n].val[1];
        fz = sample->tdatas[n].val[2];

    } else {
        i = n -1;
        j = n;
        dt = sample->tdatas[j].local_time - sample->tdatas[i].local_time;
        float a = (float)( t - sample->tdatas[i].local_time ) / dt;
        float b = 1. - a;
        fx = sample->tdatas[j].val[0]*a + sample->tdatas[i].val[0]*b;
        fy = sample->tdatas[j].val[1]*a + sample->tdatas[i].val[1]*b;
        fz = sample->tdatas[j].val[2]*a + sample->tdatas[i].val[2]*b;
    }
    return n;
}

/*
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
*/

void CXGesture::getPointShift( sensor_data *accs, int index,
                                                    float &fx, float &fy,
                                                    float &fz, int64_t &t ) {
    int limit = accs->nData - 1;
    int i = 0;
    int j = 0;
    if ( !index ) {
        fx = fy = fz = 0.0;

        t = accs->tdatas[1].local_time - accs->tdatas[0].local_time;
     //   dist = CXDFDIST(fx, fy);
    } else {
        i = limit - 1;
        j = 0;
        float vx = 0;
        float vy = 0;
        float vz = 0;
        float sx = 0;
        float sy = 0;
        float sz = 0;
/*
        while (j <= i) {
            float ax = 0.5*(accs->tdatas[j+1].val[0] + accs->tdatas[j].val[0]);
            float ay = 0.5*(accs->tdatas[j+1].val[1] + accs->tdatas[j].val[1]);
            float az = 0.5*(accs->tdatas[j+1].val[2] + accs->tdatas[j].val[2]);
            t = accs->tdatas[j+1].local_time - accs->tdatas[j].local_time;
            float ft = CXDFSCALETIME(t);
            sx = sx + vx * ft + 0.5*(ax * ft *ft);
            sy = sy + vy * ft + 0.5*(ay * ft *ft);
            sz = sz + vz * ft + 0.5*(az * ft *ft);
            m_location[j].fx = sx;
            m_location[j].fy = sy;
            m_location[j].fz = sz;
            vx = vx + ax * ft;
            vy = vy + ay * ft;
            vz = vz + az * ft;

            j++;
        }
*/
        fx = m_location[index-1].fx;
        fy = m_location[index-1].fy;
        fz = m_location[index-1].fz;
        t = accs->tdatas[index].local_time - accs->tdatas[index-1].local_time;
        //dist = CXDFDIST(fx, fy);
    }
    /*
    else {
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
    }*/

}

void CXGesture::gestureStage( cxGestureStage *stage, sensor_sample *sample, float *dist,
                                                int start, int end )
{
    sdata_packet *paccs = &sample->sensors[SENSOR_ACCS];
    sensor_TData *tdatas = paccs->sdata.tdatas;
   // cxPoint3F spt( tdatas[start].val[0], tdatas[start].val[1], tdatas[start].val[2]);
   // cxPoint3F ept( tdatas[end].val[0], tdatas[end].val[1], tdatas[end].val[2]);
    float startx = 0.f;
    float starty = 0.f;
    float startz = 0.f;

    if (start != 0) {
        startx = m_location[start -1].fx;
        starty = m_location[start -1].fy;
        startz = m_location[start -1].fz;
    }

    cxPoint3F spt(startx, starty, startz);
    cxPoint3F ept(m_location[end-1].fx, m_location[end-1].fy, m_location[end-1].fz);

    int64_t t0 = tdatas[start].local_time;
    int64_t t1 = tdatas[end].local_time;
    int64_t t = 0;
    float tmp = 0.f;
    float fx = 0.f;
    float fy = 0.f;
    float fz = 0.f;
    float ft = 0.f;
    float fval = 0.f;
    int n = 0;
    int i = 0;
    //vector
    vector( stage->vector, spt, ept );
    fprintf( stderr, "gestureStage length is %f \n",stage->vector.length);

    //distance
    stage->distance = 0.;
    n = start;
    i = start+1;
    for ( ; i <= end; i++ ) {
        fx = m_location[i-1].fx;
        fy = m_location[i-1].fy;
        fz = m_location[i-1].fz;
        float fx0 = 0.f;
        float fy0 = 0.f;
        float fz0 = 0.f;
        if (i > 1) {
            fx0 = m_location[n-1].fx;
            fy0 = m_location[n-1].fy;
            fz0 = m_location[n-1].fz;
        }
       // fx = tdatas[i].val[1] - tdatas[n].val[1];
       // fy = tdatas[i].val[2] - tdatas[n].val[2];
        //tmp = CXDFDIST( fx, fy );
        tmp = CXDFDIST3D( (fx -fx0), (fy-fy0) ,(fz-fz0));
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
 //   stage->pressure.mean = 0.f;
 //   stage->pressure.var = 0.f;
 //   stage->pressure.var2 = 0.f;
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
     //   fval = ( tdatas[i].val[0] + tdatas[n].val[0] ) * 0.5;
     //   tmp = fval * ft;
     //   stage->pressure.mean += tmp;
     //   fval *= fval;
    //    tmp = fval * ft;
     //   stage->pressure.var2 += tmp;

        n = i;
    }
    stage->deviation.mean /= stage->latency; //Mean = sigma( X ) / sigma( N(time) )
    tmp = stage->deviation.mean * stage->deviation.mean; //Mean^2
    stage->deviation.var = stage->deviation.var2;
    stage->deviation.var -= stage->latency * tmp; //sigma( X^2 ) - Mean * sigma( N(time) )
    stage->deviation.var /= stage->latency; //sigma( X-Mean)^2 / N(time)
    fprintf( stderr, "var: %f \n", stage->deviation.var);

    //pressure
//    stage->pressure.mean /= stage->latency;
//    tmp = stage->pressure.mean * stage->pressure.mean;
 //   stage->pressure.var = stage->pressure.var2;
  //  stage->pressure.var -= tmp * stage->latency;
 //   stage->pressure.var /= stage->latency;

//    sensor_data *grav = &sample->sensors[SENSOR_GRAV].sdata;
 //   sensor_data *accs = &sample->sensors[SENSOR_ACCS].sdata;
 //   sensor_data *gyro = &sample->sensors[SENSOR_GYRO].sdata;

  //  slideSensorStage( &stage->grav, grav, t0, t1 );
  //  slideSensorStage( &stage->accs, accs, t0, t1 );
  //  slideSensorStage( &stage->gyro, gyro, t0, t1 );
}


/*
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
*/
void CXGesture::combineSensorStage( cxGestureStage *dst, cxGestureStage *s1, cxGestureStage *s2 )
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
  //  dst->pressure.mean = s1->pressure.mean * s1->latency + s2->pressure.mean * s2->latency;
  //  dst->pressure.mean /= dst->latency;
  //  dst->pressure.var2 = s1->pressure.var2 + s2->pressure.var2;
 //   dst->pressure.var =  dst->pressure.var2;
  //  tmp = dst->pressure.mean * dst->pressure.mean;
 //   dst->pressure.var -= tmp * dst->latency;
 //   dst->pressure.var /= dst->latency;

    //combineDevation( &dst->grav, &s1->grav, &s2->grav, dst->latency, s1->latency, s2->latency );
  //  combineDevation( &dst->accs, &s1->accs, &s2->accs, dst->latency, s1->latency, s2->latency );
   // combineDevation( &dst->gyro, &s1->gyro, &s2->gyro, dst->latency, s1->latency, s2->latency );

}

void CXGesture::normalize()
{
    //ceshi
//    m_feature.fval[0] = m_feature.fval[57] = 111;

    cxGestureStage *s = &m_stage[gesture_full];
    cxGestureStage *front = &m_stage[gesture_front];
    cxGestureStage *back = &m_stage[gesture_back];
    cxGesturePoint *start = &m_points[gesture_start];
    cxGesturePoint *end = &m_points[gesture_end];
    cxGesturePoint *ldp = &m_points[gesture_ldp];

   // m_feature.startX = start->location.fx; //location
 //   m_feature.startY = start->location.fy;
  //  m_feature.startZ = start->location.fz;

    m_feature.endX = end->location.fx / 20.f;
    m_feature.endY = end->location.fy / 20.f;
    m_feature.endZ = end->location.fz / 20.f;
    m_feature.ldpX = ldp->location.fx / 20.f;
    m_feature.ldpY = ldp->location.fy / 20.f;
    m_feature.ldpZ = ldp->location.fz / 20.f;

    m_feature.pstartX = start->position.std.fx; //position
    m_feature.pstartY = start->position.std.fy;
    m_feature.pstartZ = start->position.std.fz;

    m_feature.pendX = end->position.std.fx;
    m_feature.pendY = end->position.std.fy;
    m_feature.pendZ = end->position.std.fz;
    m_feature.pldpX = ldp->position.std.fx;
    m_feature.pldpY = ldp->position.std.fy;
    m_feature.pldpZ = ldp->position.std.fz;

    m_feature.gstartX = start->gyro[0].fx / 10.f; //location
    m_feature.gstartY = start->gyro[0].fy / 10.f;
    m_feature.gstartZ = start->gyro[0].fz / 10.f;

    m_feature.gendX = end->gyro[0].fx / 10.f; //gyro
    m_feature.gendY = end->gyro[0].fy / 10.f;
    m_feature.gendZ = end->gyro[0].fz / 10.f;
    m_feature.gldpX = ldp->gyro[0].fx / 10.f;
    m_feature.gldpY = ldp->gyro[0].fy / 10.f;
    m_feature.gldpZ = ldp->gyro[0].fz / 10.f;
//    m_feature.gDirect = s->vector.direct; //direction
    m_feature.ldpDirectRotationX = m_stage[gesture_full].rotationX; //rotation
    m_feature.ldpDirectRotationY = m_stage[gesture_full].rotationY;
    m_feature.ldpDirectRotationZ = m_stage[gesture_full].rotationZ;

    m_feature.frontRotationX = m_stage[gesture_front].rotationX;
    m_feature.frontRotationY = m_stage[gesture_front].rotationY;
    m_feature.frontRotationZ = m_stage[gesture_front].rotationZ;

    m_feature.backRotationX = m_stage[gesture_back].rotationX;
    m_feature.backRotationY = m_stage[gesture_back].rotationY;
    m_feature.backRotationZ = m_stage[gesture_back].rotationZ;

    m_feature.gLength = s->vector.length /20.f; //vector
    m_feature.gDistance = s->distance /20.f;
    m_feature.frontLength = front->vector.length /20.f;
    m_feature.frontDistance = front->distance /20.f;
    m_feature.backLength = back->vector.length /20.f;
    m_feature.gVelocty = s->vel / 20.f; //vel
    m_feature.frontVelocity = front->vel / 20.f;
    m_feature.backVelocity = back->vel / 20.f;
    m_feature.ldpDiviation = m_ldpDist; //div

    m_feature.gDivMean = s->deviation.mean;
    m_feature.frontDivMean = front->deviation.mean;
    m_feature.backDivMean = back->deviation.mean;
 //   m_feature.gPressureMean = s->pressure.mean / m_pressureSize; //press mean
//    m_feature.frontPresMean = front->pressure.mean;
//    m_feature.backPresMean = back->pressure.mean;
    m_feature.gDivVar = s->deviation.var; //div
    m_feature.frontDivVar = front->deviation.var;
    m_feature.backDivVar = back->deviation.var;

    m_feature.startAccX = start->position.desc.accX / 32.f; //acc
    m_feature.startAccY = start->position.desc.accY / 32.f; //acc
    m_feature.startAccZ = start->position.desc.accZ / 32.f; //acc

    m_feature.endAccX = end->position.desc.accX / 32.f;
    m_feature.endAccY = end->position.desc.accY / 32.f;
    m_feature.endAccZ = end->position.desc.accZ / 32.f;

    m_feature.ldpAccX = ldp->position.desc.accX / 32.f;
    m_feature.ldpAccY = ldp->position.desc.accY / 32.f;
    m_feature.ldpAccZ = ldp->position.desc.accZ / 32.f;

//    m_feature.gPressureVar = s->pressure.var / m_pressureSize; //press var
  //  m_feature.gGravMean[0] = s->grav.mean[0];//sensor global mean
 //   m_feature.gGravMean[1] = s->grav.mean[1];
 //  m_feature.gGravMean[2] = s->grav.mean[2];
  //  m_feature.gAccsMean[0] = s->accs.mean[0];
 //   m_feature.gAccsMean[1] = s->accs.mean[1];
  //  m_feature.gAccsMean[2] = s->accs.mean[2];
  //  m_feature.gGyroMean[0] = s->gyro.mean[0];
//    m_feature.gGyroMean[1] = s->gyro.mean[1];
 //   m_feature.gGyroMean[2] = s->gyro.mean[2];

 //   m_feature.frontGyroMean[0] = front->gyro.mean[0];//gyro front, back mean
 //   m_feature.frontGyroMean[1] = front->gyro.mean[1];
 //   m_feature.frontGyroMean[2] = front->gyro.mean[2];
 //   m_feature.backGyroMean[0] = back->gyro.mean[0];
 //   m_feature.backGyroMean[1] = back->gyro.mean[1];
 //   m_feature.backGyroMean[2] = back->gyro.mean[2];
 //   m_feature.gGravVar[0] = s->grav.var[0];//sensor global var
  //  m_feature.gGravVar[1] = s->grav.var[1];
  //  m_feature.gGravVar[2] = s->grav.var[2];
 //   m_feature.gAccsVar[0] = s->accs.var[0];
 //   m_feature.gAccsVar[1] = s->accs.var[1];
  //  m_feature.gAccsVar[2] = s->accs.var[2];
  //  m_feature.gGyroVar[0] = s->gyro.var[0];
 //   m_feature.gGyroVar[1] = s->gyro.var[1];
  //  m_feature.gGyroVar[2] = s->gyro.var[2];
 //   m_feature.frontGyroVar[0] = front->gyro.var[0];//gyro front, back var
//   m_feature.frontGyroVar[1] = front->gyro.var[1];
 //   m_feature.frontGyroVar[2] = front->gyro.var[2];
 //   m_feature.backGyroVar[0] = back->gyro.var[0];
//    m_feature.backGyroVar[1] = back->gyro.var[1];
 //   m_feature.backGyroVar[2] = back->gyro.var[2];

    //normalize gravity
  //  normalize3D( m_feature.gGravMean );
   // normalize3D( m_feature.gGravVar );
}

void CXGesture::printPoint( FILE *fp, cxGesturePoint *pt )
{
    fprintf( fp, " %f %f %f \n", pt->location.fx, pt->location.fy, pt->location.fz ); //fx, fy, fz
    fprintf( fp, " %f %f %f \n", pt->position.std.fx, pt->position.std.fy, pt->position.std.fz ); //pfx, pfy, pfz
    fprintf( fp, " %f %f %f \n", pt->gyro[0].fx, pt->gyro[0].fy, pt->gyro[0].fz ); //gfx, gfy, gfz
    fprintf( fp, " %f %f %f \n", pt->position.desc.accX, pt->position.desc.accY, pt->position.desc.accZ ); //accx, accy, accz

}

void CXGesture::printStage( FILE *fp, cxGestureStage *stage )
{
    fprintf(  fp, " %f %f %f \n", stage->distance, stage->vel, stage->latency );
    fprintf(  fp, " %f %f \n", stage->deviation.mean, stage->deviation.var );
 //   fprintf(  fp, " %f %f \n", stage->pressure.mean, stage->pressure.var );
    fprintf(  fp, " %f %d %d %d \n", stage->vector.length, stage->vector.directxy, stage->vector.directyz, stage->vector.directxz );
    fprintf(  fp, " %f %f %f \n", stage->rotationX, stage->rotationY, stage->rotationZ );

}

int CXGesture::readPoint(FILE *fp, cxGesturePoint *pt)
{
    if ( fscanf( fp, " %f %f %f " , &pt->location.fx, &pt->location.fy, &pt->location.fz  ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->position.std.fx, &pt->position.std.fy, &pt->position.std.fz ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->gyro[0].fx, &pt->gyro[0].fy, &pt->gyro[0].fz ) == EOF )
        return -1;
    if ( fscanf( fp, " %f %f %f " , &pt->position.desc.accX, &pt->position.desc.accY, &pt->position.desc.accZ ) == EOF )
        return -1;

    return 0;
}

int CXGesture::readStage(FILE *fp, cxGestureStage *stage)
{
    if ( fscanf(  fp, " %f %f %f ", &stage->distance, &stage->vel, &stage->latency ) == EOF )
        return -1;
     if ( fscanf(  fp, " %f %f ", &stage->deviation.mean, &stage->deviation.var ) == EOF )
         return -1;
     if ( fscanf(  fp, " %f %d %d %d  ", &stage->vector.length, &stage->vector.directxy, &stage->vector.directyz, &stage->vector.directxz ) == EOF )
         return -1;
     if ( fscanf( fp, " %f %f %f ", stage->rotationX, stage->rotationY, stage->rotationZ ) == EOF ) //grav
         return -1;

    return 0;
}


