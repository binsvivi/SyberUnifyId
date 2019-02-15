#ifndef CXGESTURE_H
#define CXGESTURE_H

#include "cxsensordata.h"

enum {
    gesture_start = 0,
    gesture_end,
    gesture_ldp,
    gesture_numPoint,
};

enum {
    gesture_front = 0,
    gesture_back,
    gesture_full,
    gesture_numStage,
};

//#define gestureFeatureSize 57
//起点,终点位置,决定滑动区域的具体屏幕位置                                          1
//起点->终点矢量+偏移量:组成平行四边形来描述滑动轨迹几何区域       2
//运动方向旋转角度,表示运动的变化                                                           3
//运动轨迹曲线特征,曲线内接弦矢量                                                           4
//运动速度特征,时间相关                                                                               5
//偏移分布特征                                                                                               6
//压力特征                                                                                                       7
//偏移分布特征    加强                                                                                    8
//运动速度特征,时间相关      加强                                                                  9
//传感器分布 均值                                                                                          10
//传感器分布 分段均值       加强                                                                    11
//传感器分布 方差                                                                                          12
//传感器分布 方差均值       加强                                                                    13
enum {
    GESTURE_END_X = 0, //POSITION               位置,表达gesture在space的位置区域
    GESTURE_END_Y,
    GESTURE_END_Z,
    GESTURE_LDP_X,
    GESTURE_LDP_Y,
    GESTURE_LDP_Z,
    GESTURE_FACE_START_X, // FACE POSITION               位置,表达gesture在space的位置区域face
    GESTURE_FACE_START_Y,
    GESTURE_FACE_START_Z,
    GESTURE_FACE_END_X,
    GESTURE_FACE_END_Y,
    GESTURE_FACE_END_Z,
    GESTURE_FACE_LDP_X,
    GESTURE_FACE_LDP_Y,
    GESTURE_FACE_LDP_Z,
    GESTURE_GYRO_START_X, // GYRO VELOCITY               位置,表达gesture在space的位置区域角速度
    GESTURE_GYRO_START_Y,
    GESTURE_GYRO_START_Z,
    GESTURE_GYRO_END_X,
    GESTURE_GYRO_END_Y,
    GESTURE_GYRO_END_Z,
    GESTURE_GYRO_LDP_X,
    GESTURE_GYRO_LDP_Y,
    GESTURE_GYRO_LDP_Z,
    GESTURE_FACE_DIR_ROT_FULL_X, // FACE ROTATION               位置,表达gesture在space的位置区域face变化
    GESTURE_FACE_DIR_ROT_FULL_Y,
    GESTURE_FACE_DIR_ROT_FULL_Z,
    GESTURE_FACE_DIR_ROT_FRONT_X,
    GESTURE_FACE_DIR_ROT_FRONT_Y,
    GESTURE_FACE_DIR_ROT_FRONT_Z,
    GESTURE_FACE_DIR_ROT_BACK_X,
    GESTURE_FACE_DIR_ROT_BACK_Y,
    GESTURE_FACE_DIR_ROT_BACK_Z,
    GESTURE_GLOBAL_DISTANCE, //                                 轨迹距离, 矢量长度                  4
    GESTURE_GLOBAL_LENGTH, //                                 轨迹长度, 矢量长度                  4
    GESTURE_FRONT_DISTANCE, //                                 front轨迹距离, 矢量长度                  4
    GESTURE_FRONT_LENGTH, //                                 front轨迹长度, 矢量长度                  4
    GESTURE_BACK_DISTANCE, //                                 back轨迹距离, 矢量长度                  4
    GESTURE_GLOBAL_VEL, //VELOCITY                    速度分布                                        5
    GESTURE_FRONT_VEL,
    GESTURE_BACK_VEL,
    GESTURE_LDP_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    GESTURE_GLOBAL_DIV_MEAN, //VELOCITY                    速度分布                                        5
    GESTURE_FRONT_DIV_MEAN,
    GESTURE_BACK_DIV_MEAN,
    GESTURE_GLOBAL_DIV_VAR, //VELOCITY                    速度分布                                        5
    GESTURE_FRONT_DIV_VAR,
    GESTURE_BACK_DIV_VAR,
    GESTURE_GACC_START_X, // GYRO VELOCITY               位置,表达gesture在space的位置区域
    GESTURE_GACC_START_Y,
    GESTURE_GACC_START_Z,
    GESTURE_GACC_END_X,
    GESTURE_GACC_END_Y,
    GESTURE_GACC_END_Z,
    GESTURE_GACC_LDP_X,
    GESTURE_GACC_LDP_Y,
    GESTURE_GACC_LDP_Z,

    GESTURE_FEATURE_DIM_SIZE  //dim size
};

extern double gesture_feature_scale[GESTURE_FEATURE_DIM_SIZE];


//#define gestureFeatureSize 57

typedef struct gestureFeature {
    gestureFeature() {
        memset( this, 0, sizeof(float) * 57 );
    }
    union {
        struct {
            //location 9
          //  float startX;
          //  float startY;
           // float startZ;

            float endX;
            float endY;
            float endZ;

            float ldpX;
            float ldpY;
            float ldpZ;

            //position 9
            float pstartX;
            float pstartY;
            float pstartZ;

            float pendX;
            float pendY;
            float pendZ;

            float pldpX;
            float pldpY;
            float pldpZ;

            //gyro 9
            float gstartX;
            float gstartY;
            float gstartZ;

            float gendX;
            float gendY;
            float gendZ;

            float gldpX;
            float gldpY;
            float gldpZ;
            //vector direction 4
//            float gDirect; //矢量方向夹角不好比较
            float ldpDirectRotationX;
            float ldpDirectRotationY;
            float ldpDirectRotationZ;
            float frontRotationX;
            float frontRotationY;
            float frontRotationZ;

            float backRotationX;
            float backRotationY;
            float backRotationZ;

            //distance 5
            float gLength;
            float gDistance;
            float frontLength;
            float frontDistance;
            float backLength;
//            float backDistance;
            //velocity 3
            float gVelocty;
            float frontVelocity;
            float backVelocity;
            //ldp diviation 4
            float ldpDiviation;
            float gDivMean;
            float frontDivMean;
            float backDivMean;
            //presure 3
 //           float gPressureMean;
//            float frontPresMean;
//            float backPresMean;
            //diviation 3
            float gDivVar;
            float frontDivVar;
            float backDivVar;
            //accelerater 9
            float startAccX;
            float startAccY;
            float startAccZ;

            float endAccX;
            float endAccY;
            float endAccZ;

            float ldpAccX;
            float ldpAccY;
            float ldpAccZ;
//            //pressure 3
//            float gPressureVar;
//            float frontPresVar;
//            float backPresVar;

            //sensor 10 * 3
            //mean
         //   float gGravMean[3];
          //  float gAccsMean[3];
           // float gGyroMean[3];
//            float frontGravMean[3];
//            float frontAccsMean[3];
          //  float frontGyroMean[3];
//            float backGravMean[3];
//            float backAccsMean[3];
          //  float backGyroMean[3];
            //var
        //    float gGravVar[3];
        //    float gAccsVar[3];
        //    float gGyroVar[3];
//            float frontGravVar[3];
//            float frontAccsVar[3];
          //  float frontGyroVar[3];
//            float backGravVar[3];
//            float backAccsVar[3];
          //  float backGyroVar[3];
        };

        float fval[57];
    };

}gestureFeature;

typedef struct GPointDsc {
    GPointDsc() {
        vel = accX =accY = accZ = 0.f;
        direct = -1;
    }

    GPointDsc& operator = ( const GPointDsc &pt ) {
        vel = pt.vel;
        accX = pt.accX;
        accY = pt.accY;
        accZ = pt.accZ;

        direct = pt.direct;
        return *this;
    }

    float vel; //速度
    float accX; //加速度
    float accY; //加速度
    float accZ; //加速度

    float directF;
    int direct; //方向
}gesturePointDescribe;

typedef struct gPointF {
    gPointF() {
        fx = fy = pressure = 0.f;
    }
    float fx;
    float fy;
    union {
        float pressure;
        float fz;
    };
}gesturePointF;

typedef struct gestureCurvePt {
    gPointF std; //方向
    GPointDsc desc; //速度,加速度,方向
}gestureCurvePoint;

typedef struct cxGesturePoint {
    // 方向
    gestureCurvePt position;
    //传感器 值,梯度(变化)
    cxPoint3F grav[2];
    cxPoint3F accs[2];
    cxPoint3F gyro[2];
    cxPoint3F location;
} cxGesturePoint ;

typedef struct gDeviation {
    gDeviation() {
        mean = 0.f;
        var = 0.f;
    }
    union {
        struct {
            float mean;
            float var;
            float var2;
        };
    float val[3];
    };
}gestureDeviation;

typedef struct cxGestureStage {
    cxGestureStage() {
        distance = 0.f;
        vel = 0.f;
        latency = 0.f;
    }

    gDeviation deviation; //偏移属性(均值,方差)
    float distance; //曲线长度
    float vel; // 平均速度;
    gDeviation pressure; // 平均压力
    float latency; //耗时
    cxVector3d vector; //曲线端点连线矢量(长度,方向)
    float rotationX;
    float rotationY;
    float rotationZ;

    //曲线期间,传感器数据分布属性(均值,方差)
 //   sDeviation3F grav;
//    sDeviation3F accs;
 //   sDeviation3F gyro;
}cxGestureStage;

typedef struct gestureUnitDsc {
    //gesture
    struct {
        cxIntF pressure;
        cxIntF vel;
        cxIntF acc;
        cxIntF distance;
        cxIntF rotation;
        float ldpDistance;
    } gesture;

    //sensor
    cxVecInt3D grav[2];
    cxVecInt3D accs[2];
    cxVecInt3D gyro[2];

    //time
    float latency;
}gestureUnitDescribe;

class CXGesture
{
public:

    CXGesture();
    ~CXGesture();
    void clear();

    int loadSample(  sensor_sample *sample );
    void save( const char *filepath );
    void saveFeat( const char *filepath );
    int loadFile( const char *filepath );
    int loadFeature( const char *filepath );


protected:
    int findLdpPosition( sensor_data *sample, float *distances, cxLine3dDesc &line );
    void gesturePoint( cxGesturePoint *pt, sensor_sample *sample, int index );
    bool getRotationMatrix( float *r, float *I, float *gravity, float * geomagnetic);
    void calculateOrientation(sensor_data * accs_data, int index, float * gvalues, float * magneticfieldvalues, float * tmp);
    void getOrientation(float * R, float * values);
    // void gestureSensorPoint( cxGesturePoint *pt, sdata_packet *sample, int index );
    void gestureSensorPoint( cxGesturePoint *pt, sensor_sample *sample, int index );
   // void interSensorPoint( cxPoint3F *pt, sdata_packet *sample, int64_t t );
    int interSensorPoint( sensor_data *sample, int64_t t , float& fx, float& fy, float& fz);

    void getPointShift( sensor_data*accs, int index,
                                                            float &fx, float &fy,
                                                            float &fz, int64_t &t );
    void getSensorShift( cxPoint3F *pt, sensor_data*sensor, int pri, int next, float a, float b );

    void gestureStage( cxGestureStage *stage, sensor_sample *sample, float *dist, int start, int end );
 //   void gestureSensorStage( sDeviation3F *dev, sensor_data *sample, int64_t t0, int64_t t1 );
    void combineSensorStage( cxGestureStage *dst, cxGestureStage *s1, cxGestureStage *s2 );

    void normalize();

    void printPoint( FILE *fp, cxGesturePoint *pt );
    void printStage( FILE *fp, cxGestureStage *stage );

    int readPoint( FILE *fp, cxGesturePoint *pt );
    int readStage( FILE *fp, cxGestureStage *stage );

public:
    static float m_screenSize;
    static float m_pressureSize;
    static float m_velocitySize;
    static float m_accelerateSize;
protected:
//    cxArcF m_arc;
    cxGesturePoint m_points[gesture_numPoint];
    cxGestureStage m_stage[gesture_numStage];
 //   slideUnitDsc m_sDesc;
    gestureUnitDsc m_sDesc;
    float m_ldpDist;
    cxPoint3F * m_location;
    gestureFeature m_feature;
};

void featGestureSample( CXGesture *slide, sensor_sample *sample, const char *featPath );
void  featGestures( CXSensorSample *sampler, const char* samplePath, const char *featPath );
void ceshiGesture();

#endif // CXGESTURE_H
