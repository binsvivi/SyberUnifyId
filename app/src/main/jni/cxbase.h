#ifndef CXBASE_H
#define CXBASE_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>

//#define CXDF_QT
#define CXDF_CESHI

#define CXDFCLEARFREE( a )   if ( a ) { \
                                                            a->clear(); \
                                                            free( a ); \
                                                            a = NULL; \
                                                        }

#define CXDFFREE( a )   if ( a ) { \
                                                free( a ); \
                                                a = NULL; \
                                            }
#define CXDFMATFREE( a, n )   if ( a ) { \
                                                for ( int i = 0; i < n; i++ ) \
                                                    free( a[i] ); \
                                                free( a ); \
                                                a = NULL; \
                                            }

#define CXDFDELETE( a )   if ( a ) { \
                                                delete a; \
                                                a = NULL; \
                                            }

class memManager{
public:
    memManager() {}
    static void print();
    static int refInt;
    static int refFloat;
    static int refDouble;
    static int refPointer;
};

void** MallocPointers( int n );

void FreePointers( void **ptr );

int* MallocInt( int n );
void FreeInt( int *ptr );
float* MallocFloat( int n );
void FreeFloat( float *ptr );
double* MallocDouble( int n );
void FreeDouble( double *ptr );

#ifdef CXDF_CESHI

#define CXDFMallocPoints(type,a) (type **)( MallocPointers(a) )
#define CXDFFreePointers( a ) FreePointers( (void**)a ); a = NULL;

#define CXDFMallocInt( a ) MallocInt( a )
#define CXDFMallocFloat( a ) MallocFloat( a )
#define CXDFMallocDouble( a ) MallocDouble( a )

#define CXDFFreeInt( a ) FreeInt( a );  a = NULL
#define CXDFFreeFloat( a ) FreeFloat( a );  a = NULL
#define CXDFFreeDouble( a ) FreeDouble( a );  a = NULL

#else

#define CXDFMallocPoints(type,a) (type **)malloc( sizeof(void*) *a )
#define CXDFFreePointers( a ) CXDFFREE( a )

#define CXDFMallocInt( a ) (int*)malloc( sizeof(int) *a )
#define CXDFMallocFloat( a ) (float*)malloc( sizeof(float) *a )
#define CXDFMallocDouble( a ) (double*)malloc( sizeof(double) *a )

#define CXDFFreeInt( a ) CXDFFREE( a )
#define CXDFFreeFloat( a ) CXDFFREE( a )
#define CXDFFreeDouble( a ) CXDFFREE( a )

#endif


extern const float MIN_FVAR;

enum {
    line_normal,
    line_vertical,
    line_horizontal,
};

enum {
    CXEN_TEX_LINE = 0,
    CXEN_TEX_POINT,
    CXEN_TEX_TRIANGLEX,
//    CXEN_TEX_TRIANGLEY,
    CXEN_TEX_NUM
};

enum {
    CXEN_BASE_CLASS = 0,
};

enum SensorType {
    SENSOR_TOUCH = 0,       //touch
    SENSOR_GRAV,                //gravity
    SENSOR_ACCS,                //accerleration
    SENSOR_GYRO,               //gyroscope
    SENSOR_MAGN,               //magnetics
    SENSOR_NUM
};

enum {
    feature_std = 0,
    feature_del,
    feature_acc,
    feature_num,
};

struct cxPointF;
struct cxPoint3F;

bool isEqualF( float fval1, float fval2 );

typedef struct SCSize {
    SCSize() {
        min = 0.;
        max = 0.;
        size = -1.;
    }
    void clear() {
        min = 0.;
        max = 0.;
        size = -1.;
    }

    union {
        struct {
            double min;
            double max;
            double size;
        };
        struct {
            double val[3];
        };
    };
}SCSize;

typedef struct SCTimeSize {
    SCTimeSize() {
        minT = maxT = sizeT = 0;
        local.minT = local.maxT = local.sizeT = 0;
        local.baseTime = -1;
    }
    void clear() {
        minT = maxT = sizeT = 0;
        local.minT = local.maxT = local.sizeT = 0;
        local.baseTime = -1;
    }

    union {
        struct {
            int64_t minT;
            int64_t maxT;
            int64_t sizeT;
        };
        struct {
            int64_t val[3];
        };
    };

    struct {
        union {
            struct {
                int64_t minT;
                int64_t maxT;
                int64_t sizeT;
            };
            struct {
                int64_t val[3];
            };
        };
        int64_t baseTime;
    } local;


}SCTimeSize;


typedef struct cxPointF {
    cxPointF() {
        fx = 0.f;
        fy = 0.f;
    }
    cxPointF( float _x, float _y ) {
        fx = _x;
        fy = _y;
    }

    cxPointF& operator = ( const cxPointF &pt ) {
        fx = pt.fx;
        fy = pt.fy;
        return *this;
    }

    union {
        struct {
            float fx;
            float fy;
        };
        struct {
            float length;
            float width;
        };
        struct {
            float wide;
            float height;
        };
        float fval[2];
    };

}cxPointF;

typedef struct cxPoint3F {
    cxPoint3F() {
        clear();
    }
    void clear() {
        fx = fy = fz = 0.;
    }

    cxPoint3F( float _x, float _y , float _z) {
        fx = _x;
        fy = _y;
        fz = _z;
    }

    cxPoint3F& operator = ( const cxPoint3F &pt ) {
        fx = pt.fx;
        fy = pt.fy;
        fz = pt.fz;
        return *this;
    }

    cxPoint3F& operator -= ( const cxPoint3F &pt ) {
        fx -= pt.fx;
        fy -= pt.fy;
        fz -= pt.fz;
        return *this;
    }

    const cxPoint3F operator - ( const cxPoint3F &pt ) {
        cxPoint3F ret = *this;
        ret.fx -= pt.fx;
        ret.fy -= pt.fy;
        ret.fz -= pt.fz;
        return ret;
    }

    union {
        struct {
            float fx;
            float fy;
            float fz;
        };
        float fval[3];
    };
}cxPoint3F;

typedef struct cxLineDesc {
    cxLineDesc() {
        k = b = fx = fy = 0.f;
        type = line_normal;
    }

    float k;
    float b;
    float fx;
    float fy;
    int type;
}cxLineDesc;

typedef struct cxLine3dDesc {
    cxLine3dDesc() {
        m = n = p = t = fx = fy = fz = 0.f;
        type = line_normal;
    }

    float m;
    float n;
    float p;
    float t;
    float fx;
    float fy;
    float fz;
    int type;
}cxLine3dDesc;

typedef struct cxLineF {
    cxLineF() {
        construct();
    }
    void construct() {
         start = vals;
         end = start + 1;
         rect = end + 1;
         length = 0.f;
     }
    cxPointF *start;
    cxPointF *end;
    cxPointF *rect;
    cxPointF vals[3];
    float length;
} cxLineF;

typedef struct cxArcF {
    cxArcF() {
        construct();
    }
    void construct() {
        start = vals;
        end = start + 1;
        ldp = end + 1;
        arcRect = ldp + 1;
        length[0] = length[1] = length[2] = 0.f;
    }

    cxPointF *start; //start point
    cxPointF *end; //end point
    cxPointF *ldp; //largest deviation point
    cxPointF *arcRect; //length, wide
    cxPointF vals[4];
    float length[2]; //start
}cxArcF;

typedef struct cxVector3d {
    cxVector3d() {
        init();
    }
    void init() {
        length = 0.f;
        directFxy = directFyz = directFxz = 0.f;
        directxy = directxz = directyz = -1;
    }

    float length;
    float directFxy;
    float directFyz;
    float directFxz;
    int directxy;
    int directyz;
    int directxz;
}cxVector3d;

typedef struct cxVector {
    cxVector() {
        init();
    }
    void init() {
        length = 0.f;
        directF = 0.f;
        direct = -1;
    }

    float length;
    float directF;
    int direct;
}cxVector;

typedef struct cxLVector {
    cxLVector() {
        construct();
    }

    void construct() {
        start = pts;
        end = start + 1;
    }

    cxPointF *start;
    cxPointF *end;

    cxVector vector;
    cxPointF pts[2];
}cxLineVector;

typedef struct SFeature {
    SFeature() {
        construct();
    }
    void construct() {
        std = feats;
        del = std + 1;
        acc = del + 1;
    }
    void clear() {
        memset( feats, 0, sizeof(cxPoint3F) * feature_num );
    }
    cxPointF position() {
        return cxPointF( std->fx, std->fy );
    }
    float pressure() {
        return std->fz;
    }
    cxPointF velocity() {
        return cxPointF( del->fx, del->fy );
    }
    float delPressure() {
        return del->fz;
    }
    cxPointF accelerate() {
        return cxPointF( acc->fx, acc->fy );
    }
    float accPressure() {
        return del->fz;
    }
    cxPoint3F *std;
    cxPoint3F *del;
    cxPoint3F *acc;
    cxPoint3F feats[feature_num];
}SensorFeature;

typedef struct cxFeature{
    cxFeature() {}
    void clear() {
        for ( int i = 0; i < SENSOR_NUM; i++ )
            features[i].clear();
    }

    SensorFeature features[SENSOR_NUM];
}cxFeature;

typedef struct cxIntF {
    cxIntF() {
        min = max = 0.f;
    }
    cxIntF( float fval ) {
        min = max = fval;
    }
    void reset( float fval ) {
        min = max = fval;
    }
    void compare( float fval ) {
        if ( min > fval )
            min = fval;
        if ( max <  fval )
            max = fval;
    }

    float min;
    float max;
}cxIntervalF;

typedef struct cxVecInt2D {
    cxVecInt2D() {}
    cxIntF fx;
    cxIntF fy;
} cxVectorInterval2D;

typedef struct cxVecInt3D {
    cxVecInt3D() {}
    void reset( float _x, float _y, float _z ) {
        fx.reset( _x );
        fy.reset( _y );
        fz.reset( _z );
    }

    void reset( float *fval ) {
        fx.reset( fval[0] );
        fy.reset( fval[1] );
        fz.reset( fval[2] );
    }

    void compare( float _x, float _y, float _z ) {
        fx.compare( _x );
        fy.compare( _y );
        fz.compare( _z );
    }

    void compare( float *fval ) {
        fx.compare( fval[0] );
        fy.compare( fval[1] );
        fz.compare( fval[2] );
    }

    cxIntF fx;
    cxIntF fy;
    cxIntF fz;
} cxVectorInterval3D;


//class CXBaseClass
//{
//public:
//    CXBaseClass() {}
//    virtual void clear() {}
//    virtual int classType() {
//        return 0;
//    }
//};

#endif // CXBASE_H
