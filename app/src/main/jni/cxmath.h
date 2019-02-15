#ifndef CXMATH_H
#define CXMATH_H

#include "cxbase.h"

#define CXDFDIST( x, y )  sqrt( x*x + y*y );
#define CXDFDIST3D( x, y, z )  sqrt( x*x + y*y + z*z);

#define CXDFSCALETIME( x )  x * 1E-9;

//point to line distance with angle
float distanceEx( float fx, float fy,
                                cxLineDesc &line );
float distance3PEx( float fx, float fy,
                                float st_x, float st_y,
                                float end_x, float end_y );
float distanceEx( float fx, float fy,
                float k, float b );
float distanceEx( float fx, float fy,
                float k, float b, float ax, float ay );

//point to line distance
float distance( float fx, float fy,
                                cxLineDesc &line );
float distance3P( float fx, float fy,
                            float st_x, float st_y,
                            float end_x, float end_y, int ltype );
float distance( float fx, float fy,
                float k, float b );
float distance( float fx, float fy,
                float k, float b, float &tx, float &ty );
float distance( float fx, float fy, float fz,
                                cxLine3dDesc &line );
void intersection( float k, float b, float _k, float _b, float &tx, float &ty );

int getTAngle( float dx, float dy );
int getTanAngle( float tan );
void vector( cxVector3d &vector, cxPoint3F &st, cxPoint3F &ed );
void vector( cxVector &vector, cxPointF &st, cxPointF &ed );
void vector( cxLVector &lvector, cxPointF &st, cxPointF &ed );

int rotation ( int angle0, int angle1 );
float rotationf ( float angle0, float angle1 );

void normalize3D( float *fval );

class cxmath
{
public:
    cxmath();
};

#endif // CXMATH_H
