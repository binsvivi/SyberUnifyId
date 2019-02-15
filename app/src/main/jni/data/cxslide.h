#ifndef CXSLIDE_H
#define CXSLIDE_H

#include "cxsensordata.h"

enum {
    slide_start = 0,
    slide_end,
    slide_ldp,
    slide_ldp4,
    slide_ldp5,
    slide_ldp6,
    slide_ldp7,
    slide_ldp8,
    slide_ldp9,
    slide_ldp10,
    slide_ldp11,
    slide_numPoint,
};

enum {
    slide_front = 0,
    slide_back,
    slide_touch,
    slide_touch4,
    slide_touch5,
    slide_touch6,
    slide_touch7,
    slide_touch8,
    slide_touch9,
    slide_touch10,
    slide_numStage,
};

enum SlideSensor {
    SLIDE_ACC_GRAV,  //accerleration
    SLIDE_GYRO,         //gyroscope
    SLIDE_NUM
};

//#define slideFeatureSize 43
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
    SLIDE_START_X = 0, //POSITION               位置,表达滑动在屏幕的位置区域  1, 2
    SLIDE_START_Y,
    SLIDE_END_X,
    SLIDE_END_Y,
    SLIDE_LDP_X,
    SLIDE_LDP_Y,
    SLIDE_LDP1_X,
    SLIDE_LDP1_Y,
    SLIDE_LDP2_X,
    SLIDE_LDP2_Y,
    SLIDE_LDP3_X,
    SLIDE_LDP3_Y,
    SLIDE_LDP4_X,
    SLIDE_LDP4_Y,
    SLIDE_LDP5_X,
    SLIDE_LDP5_Y,
    SLIDE_LDP6_X,
    SLIDE_LDP6_Y,
    SLIDE_LDP7_X,
    SLIDE_LDP7_Y,
    SLIDE_LDP8_X,
    SLIDE_LDP8_Y,
    SLIDE_FRONT_ROT, //                                  矢量方向旋转角度, 方向相关          3
    SLIDE_BACK_ROT,
    SLIDE_LDP1_ROT,
    SLIDE_LDP2_ROT,
    SLIDE_LDP3_ROT,
    SLIDE_LDP4_ROT,
    SLIDE_LDP5_ROT,
    SLIDE_LDP6_ROT,
    SLIDE_LDP7_ROT,
    SLIDE_GLOBAL_ROT,
    SLIDE_GLOBAL_LENGTH, //                                 轨迹长度, 矢量长度                  4
//    SLIDE_GLOBAL_DISTANCE,
    SLIDE_FRONT_LENGTH,
    SLIDE_FRONT_DISTANCE,
    SLIDE_BACK_LENGTH,
    SLIDE_BACK_DISTANCE,
    SLIDE_LDP1_LENGTH,
    SLIDE_LDP1_DISTANCE,
    SLIDE_LDP2_LENGTH,
    SLIDE_LDP2_DISTANCE,
    SLIDE_LDP3_LENGTH,
    SLIDE_LDP3_DISTANCE,
    SLIDE_LDP4_LENGTH,
    SLIDE_LDP4_DISTANCE,
    SLIDE_LDP5_LENGTH,
    SLIDE_LDP5_DISTANCE,
    SLIDE_LDP6_LENGTH,
    SLIDE_LDP6_DISTANCE,
    SLIDE_LDP7_LENGTH,
    SLIDE_LDP7_DISTANCE,
    SLIDE_GLOBAL_VEL, //VELOCITY                    速度分布                                        5
    SLIDE_FRONT_VEL,
    SLIDE_BACK_VEL,
    SLIDE_LDP1_VEL,
    SLIDE_LDP2_VEL,
    SLIDE_LDP3_VEL,
    SLIDE_LDP4_VEL,
    SLIDE_LDP5_VEL,
    SLIDE_LDP6_VEL,
    SLIDE_LDP7_VEL,
    SLIDE_LDP_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_LDP1_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_LDP2_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_LDP3_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_LDP4_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_LDP5_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_LDP6_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_LDP7_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_LDP8_DIV, //DIVIATION                          最大偏移, 偏移均值分布                6
    SLIDE_GLOBAL_DIV_MEAN,
    SLIDE_FRONT_DIV_MEAN,
    SLIDE_BACK_DIV_MEAN,
    SLIDE_GLOBAL_PRESS, //PRESSRURE             压力                                                  7
//    SLIDE_FRONT_PRESS,
//    SLIDE_BACK_PRESS,
    SLIDE_GLOBAL_DIV_VAR, //DIVIATION VAR   偏移方差分布                                  8
    SLIDE_FRONT_DIV_VAR,
    SLIDE_BACK_DIV_VAR,
    SLIDE_LDP1_DIV_VAR,
    SLIDE_LDP2_DIV_VAR,
    SLIDE_LDP3_DIV_VAR,
    SLIDE_LDP4_DIV_VAR,
    SLIDE_LDP5_DIV_VAR,
    SLIDE_LDP6_DIV_VAR,
    SLIDE_LDP7_DIV_VAR,
    SLIDE_START_ACC, //SLIDE ACCELERATE         特征点加速度                                  9
    SLIDE_END_ACC,
    SLIDE_LDP_ACC,
    SLIDE_LDP1_ACC,
    SLIDE_LDP2_ACC,
    SLIDE_LDP3_ACC,
    SLIDE_LDP4_ACC,
    SLIDE_LDP5_ACC,
    SLIDE_LDP6_ACC,
    SLIDE_LDP7_ACC,
    SLIDE_LDP8_ACC,
//    SLIDE_GLOBAL_PRESS_VAR, //PRESSURE VAR
//    SLIDE_FRONT_PRESS_VAR,
//    SLIDE_BACK_PRESS_VAR                                                
    SLIDE_GLOBAL_GRAV_MEAN_X, //GLOBAL GRAVITY MEAN                                     10      传感器数据分布
    SLIDE_GLOBAL_GRAV_MEAN_Y,
    SLIDE_GLOBAL_GRAV_MEAN_Z,
//    SLIDE_GLOBAL_ACCS_MEAN_X, //GLOBAL ACCELERATE MEAN                            考虑均值=0, 去掉背景噪音
//    SLIDE_GLOBAL_ACCS_MEAN_Y,
//    SLIDE_GLOBAL_ACCS_MEAN_Z,
    SLIDE_GLOBAL_GYRO_MEAN_X, //GLOBAL GYRO MEAN
    SLIDE_GLOBAL_GYRO_MEAN_Y,
    SLIDE_GLOBAL_GYRO_MEAN_Z,
//    SLIDE_FRONT_GRAV_MEAN_X, //RRONT GRAV MEAN                                            11
//    SLIDE_FRONT_GRAV_MEAN_Y,
//    SLIDE_FRONT_GRAV_MEAN_Z,
//    SLIDE_FRONT_ACCS_MEAN_X, //RRONT ACCS MEAN
//    SLIDE_FRONT_ACCS_MEAN_Y,
//    SLIDE_FRONT_ACCS_MEAN_Z,
//    SLIDE_FRONT_GYRO_MEAN_X, //RRONT GYRO MEAN
//    SLIDE_FRONT_GYRO_MEAN_Y,
//    SLIDE_FRONT_GYRO_MEAN_Z,
//    SLIDE_BACK_GRAV_MEAN_X, //BACK GRAV MEAN
//    SLIDE_BACK_GRAV_MEAN_Y,
//    SLIDE_BACK_GRAV_MEAN_Z,
//    SLIDE_BACK_ACCS_MEAN_X, // BACK ACCS MEAN
//    SLIDE_BACK_ACCS_MEAN_Y,
//    SLIDE_BACK_ACCS_MEAN_Z,
//    SLIDE_BACK_GYRO_MEAN_X,// BACK GYRO MEAN
//    SLIDE_BACK_GYRO_MEAN_Y,
//    SLIDE_BACK_GYRO_MEAN_Z,
    SLIDE_GLOBAL_GRAV_VAR_X, //GLOBAL GRAVITY VAR                                               12
    SLIDE_GLOBAL_GRAV_VAR_Y,
    SLIDE_GLOBAL_GRAV_VAR_Z,
    SLIDE_GLOBAL_ACCS_VAR_X, //GLOBAL ACCELERATE VAR
    SLIDE_GLOBAL_ACCS_VAR_Y,
    SLIDE_GLOBAL_ACCS_VAR_Z,
    SLIDE_GLOBAL_GYRO_VAR_X, //GLOBAL GYRO VAR
    SLIDE_GLOBAL_GYRO_VAR_Y,
    SLIDE_GLOBAL_GYRO_VAR_Z,
//    SLIDE_FRONT_GRAV_VAR_X, //RRONT GRAV VAR                                                      13
//    SLIDE_FRONT_GRAV_VAR_Y,
//    SLIDE_FRONT_GRAV_VAR_Z,
//    SLIDE_FRONT_ACCS_VAR_X, //RRONT ACCS VAR
//    SLIDE_FRONT_ACCS_VAR_Y,
//    SLIDE_FRONT_ACCS_VAR_Z,
//    SLIDE_FRONT_GYRO_VAR_X, //RRONT GYRO VAR
//    SLIDE_FRONT_GYRO_VAR_Y,
//    SLIDE_FRONT_GYRO_VAR_Z,
//    SLIDE_BACK_GRAV_VAR_X, //BACK GRAV VAR
//    SLIDE_BACK_GRAV_VAR_Y,
//    SLIDE_BACK_GRAV_VAR_Z,
//    SLIDE_BACK_ACCS_VAR_X, // BACK ACCS VAR
//    SLIDE_BACK_ACCS_VAR_Y,
//    SLIDE_BACK_ACCS_VAR_Z,
//    SLIDE_BACK_GYRO_VAR_X,// BACK GYRO VAR
//    SLIDE_BACK_GYRO_VAR_Y,
//    SLIDE_BACK_GYRO_VAR_Z,

    SLIDE_FEATURE_DIM_SIZE  //dim size
};

extern double slide_feature_scale[SLIDE_FEATURE_DIM_SIZE];

typedef struct slideFeature {
    slideFeature() {
        memset( this, 0, sizeof(float) * SLIDE_FEATURE_DIM_SIZE );
    }
    union {
        struct {
            //position 6
            float startX;
            float startY;
            float endX;
            float endY;
            float ldpX;
            float ldpY;
            float ldp1X;
            float ldp1Y;
            float ldp2X;
            float ldp2Y;
            float ldp3X;
            float ldp3Y;
            float ldp4X;
            float ldp4Y;
            float ldp5X;
            float ldp5Y;
            float ldp6X;
            float ldp6Y;
            float ldp7X;
            float ldp7Y;
            float ldp8X;
            float ldp8Y;
            //vector direction 4
//            float gDirect; //矢量方向夹角不好比较
            float frontRotation;
            float backRotation;
            float ldp1Rotation;
            float ldp2Rotation;
            float ldp3Rotation;
            float ldp4Rotation;
            float ldp5Rotation;
            float ldp6Rotation;
            float ldp7Rotation;
            float gRotation;
            //distance 5
            float gLength;
//            float gDistance;
            float frontLength;
            float frontDistance;
            float backLength;
            float backDistance;
            float ldp1Length;
            float ldp1Distance;
            float ldp2Length;
            float ldp2Distance;
            float ldp3Length;
            float ldp3Distance;
            float ldp4Length;
            float ldp4Distance;
            float ldp5Length;
            float ldp5Distance;
            float ldp6Length;
            float ldp6Distance;
            float ldp7Length;
            float ldp7Distance;

            //velocity 3
            float gVelocty;
            float frontVelocity;
            float backVelocity;
            float ldp1Velocity;
            float ldp2Velocity;
            float ldp3Velocity;
            float ldp4Velocity;
            float ldp5Velocity;
            float ldp6Velocity;
            float ldp7Velocity;

            //ldp diviation 4
            float ldpDiviation;
            float ldp1Diviation;
            float ldp2Diviation;
            float ldp3Diviation;
            float ldp4Diviation;
            float ldp5Diviation;
            float ldp6Diviation;
            float ldp7Diviation;
            float ldp8Diviation;

            float gDivMean;
            float frontDivMean;
            float backDivMean;
            //presure 3
            float gPressureMean;
//            float frontPressMean;
//            float backPressMean;
            //diviation 3
            float gDivVar;
            float frontDivVar;
            float backDivVar;
            float ldp1DivVar;
            float ldp2DivVar;
            float ldp3DivVar;
            float ldp4DivVar;
            float ldp5DivVar;
            float ldp6DivVar;
            float ldp7DivVar;

            //accelerater 3
            float startAcc;
            float endAcc;
            float ldpAcc;
            float ldp1Acc;
            float ldp2Acc;
            float ldp3Acc;
            float ldp4Acc;
            float ldp5Acc;
            float ldp6Acc;
            float ldp7Acc;
            float ldp8Acc;

//            //pressure 3
//            float gPressureVar;
//            float frontPressVar;
//            float backPressVar;
            //sensor 10 * 3
            //mean
            float gGravMean[3];
//            float gAccsMean[3];
            float gGyroMean[3];

//            float frontGravMean[3];
//            float frontAccsMean[3];
//            float frontGyroMean[3];
//            float backGravMean[3];
//            float backAccsMean[3];
//            float backGyroMean[3];
            //var
            float gGravVar[3];
            float gAccsVar[3];
            float gGyroVar[3];
//            float frontGravVar[3];
//            float frontAccsVar[3];
//            float frontGyroVar[3];
//            float backGravVar[3];
//            float backAccsVar[3];
//            float backGyroVar[3];
        };

        float fval[SLIDE_FEATURE_DIM_SIZE];
    };

}slideFeature;


typedef struct tPointF {
    tPointF() {
        fx = fy = pressure = 0.f;
    }
    float fx;
    float fy;
    union {
        float pressure;
        float fz;
    };
}touchPointF;

typedef struct TPointDsc {
    TPointDsc() {
        vel = acc = 0.f;
        direct = -1;
    }

    TPointDsc& operator = ( const TPointDsc &pt ) {
        vel = pt.vel;
        acc = pt.acc;
        direct = pt.direct;
        return *this;
    }

    float vel; //速度
    float acc; //加速度
    float directF;
    int direct; //方向
}touchPointDescribe;

typedef struct slidTouchPt {
    tPointF std; //传感器,位置,压力
    TPointDsc desc; //速度,加速度,方向
}slidTouchPoint;

typedef struct cxSlidePoint {
    // 触摸位置压力速度方向
    slidTouchPt touch;
    //传感器 值,梯度(变化)
    cxPoint3F grav[2];
    cxPoint3F accs[2];
    cxPoint3F gyro[2];
    float t;
} cxSlidePoint ;

typedef struct sDeviation {
    sDeviation() {
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
}slideDeviation;

typedef struct sDeviation3F {
    sDeviation3F() {
        mean[0] = mean[1] = mean[2 ]= 0.f;
        var[0] = var[1] = var[2] = 0.f;
        var2[0] = var2[1] = var2[2] = 0.f;
    }
    union {
        struct {
            float mean[3];
            float var[3];
            float var2[3];
        };
    float val[3][3];
    };
}slideDeviation3F;

typedef struct cxSlideStage {
    cxSlideStage() {
        distance = 0.f;
        vel = 0.f;
        latency = 0.f;
    }

    sDeviation deviation; //偏移属性(均值,方差)
    float distance; //曲线长度
    float vel; // 平均速度;
    sDeviation pressure; // 平均压力
    float latency; //耗时
    cxVector vector; //曲线端点连线矢量(长度,方向)
    float rotation; //曲线起始到终点,切线方向旋转角度
    //曲线期间,传感器数据分布属性(均值,方差)
    sDeviation3F grav;
    sDeviation3F accs;
    sDeviation3F gyro;
}cxSlideStage;

typedef struct slideUnitDsc {
    //touch
    struct {
        cxIntF pressure;
        cxIntF vel;
        cxIntF acc;
        cxIntF distance;
        cxIntF rotation;
        float ldpDistance[9];
    } touch;

    //sensor
    cxVecInt3D grav[2];
    cxVecInt3D accs[2];
    cxVecInt3D gyro[2];

    //time
    float latency;
}slideUnitDescribe;

class CXSlide
{
public:

    CXSlide();
    ~CXSlide();
    void clear();

    int loadSample(  sensor_sample *sample );
    void save( const char *filepath );
    void saveFeat( const char *filepath );
    int loadFile( const char *filepath );
    int loadFeature( const char *filepath );

protected:
    int findLdpPosition( sensor_data *sample, float *distances, cxLineDesc &line );
    int findLdpPosition( sensor_data *sample, float *distances, cxLineDesc &line , int start, int end);

    void slidePoint( cxSlidePoint *pt, sensor_sample *sample, int index );
    void slideTouchPoint( cxSlidePoint *pt, sdata_packet *sample, int index );
    void slideSensorPoint( cxSlidePoint *pt, sensor_sample *sample, int index );
    void interSensorPoint( cxPoint3F *pt, sdata_packet *sample, int64_t t );
    void getPointShift( sensor_data*touch, int index,
                                                            float &fx, float &fy,
                                                            float &dist, int64_t &t );
    void getSensorShift( cxPoint3F *pt, sensor_data*sensor, int pri, int next, float a, float b );

    void slideStage( cxSlideStage *stage, sensor_sample *sample, float *dist, int start, int end );
    int slideSensorStage( sDeviation3F *dev, sensor_data *sample, int64_t t0, int64_t t1 );
    void combineSensorStage( cxSlideStage *dst, cxSlideStage *s1, cxSlideStage *s2 );

    void normalize();

    void printPoint( FILE *fp, cxSlidePoint *pt );
    void printStage( FILE *fp, cxSlideStage *stage );

    int readPoint( FILE *fp, cxSlidePoint *pt );
    int readStage( FILE *fp, cxSlideStage *stage );

public:
    static float m_screenSize;
    static float m_pressureSize;
    static float m_velocitySize;
    static float m_accelerateSize;

protected:
//    cxArcF m_arc;
    cxSlidePoint m_points[slide_numPoint];
    cxSlideStage m_stage[slide_numStage];
    cxSlidePoint * m_output_points;
    int sum_po;
    slideUnitDsc m_sDesc;
    float m_ldpDist[9];

    slideFeature m_feature;

};

int featSlideSample( CXSlide *slide, sensor_sample *sample, const char *featPath );
void  featSlides( CXSensorSample *sampler, const char* samplePath, const char *featPath );
void ceshiSlide();

#endif // CXSLIDE_H
