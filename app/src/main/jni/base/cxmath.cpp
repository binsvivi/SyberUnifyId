#include "cxmath.h"

float distanceEx( float fx, float fy,
                                cxLineDesc &line ) {
    if ( line.type == line_normal )
        return distanceEx( fx, fy, line.k, line.b, line.fx, line.fy );
    else if ( line.type == line_vertical ) {
        float dist = 0.f;
        if ( line.fy > 0. ) // ^
            dist = fx - line.fx;
        else //v
            dist = line.fx - fx;
        return dist;
    } else { //horizontal line
        float dist = 0.f;
        if ( line.fx > 0.) //->
            dist = fy - line.fy;
        else
            dist = line.fy - fy;
        return dist;
    }
}

float distance3PEx( float fx, float fy,
                                float st_x, float st_y,
                                float end_x, float end_y ) {

    cxLineDesc line;
    bool bfV = false;
    bool bfH = false;
    if ( st_x == end_x )
        bfV = true;
    if ( st_y == end_y )
        bfH = true;

    float dx = 0.f;
    float dy = 0.f;
    float dist = 0.f;
    if ( bfV && bfH ) { //point
        dx = fx - st_x;
        dy = fy - st_y;
        dist = sqrt( dx*dx + dy*dy );
        return dist;
    } else if ( bfV ) {
        line.type = line_vertical;
        line.fx = st_x;
        if ( end_y > st_y )
            line.fy = 1.;
        else
            line.fy = -1.;
    } else if ( bfH ) {
        line.type = line_horizontal;
        line.fy = st_y;
        if ( end_x > st_x )
            line.fx = 1.;
        else
            line.fx = -1.;
    } else {
        line.type = line_normal;
        line.fx = end_x - st_x;
        line.fy = end_y - st_y;
        line.k = line.fy / line.fx;
        line.b = st_y - line.k * st_x; //y - kx = b;
    }

    return distanceEx( fx, fy, line );
}

float distanceEx( float fx, float fy,
                float k, float b, float ax, float ay ) {
    float tx = 0.;
    float ty = 0.;
    float dist = distance( fx, fy, k, b, tx, ty );
    int mode = 0;
    int val = 0.;
    float d = 0.;
    if ( k > 0. ) {
        if ( ax > 0. ) { //quadrand I
            if ( k < 1. )
                mode = line_horizontal;
            else
                mode = line_vertical;
            val = 1.;
        } else { //quadrand III
            if ( k < 1. )
                mode = line_horizontal;
            else
                mode = line_vertical;
            val = -1.;
        }
    } else {
        if ( ax < 0. ) { //quadrand II
            if ( k > -1. ) {
                mode = line_horizontal;
                val = -1.;
            } else  {
                mode = line_vertical;
                val = 1.;
            }
        } else { //quadrand IV
            if ( k > -1. ) {
                mode = line_horizontal;
                val = 1.;
            } else {
                mode = line_vertical;
                val = -1.;
            }
        }
    }

    if ( mode == line_horizontal ) {
        d = fy - ty;
        d *= val;
        if ( d < 0. )
            return -dist;
        else
            return dist;
    } else {
        d = fx - tx;
        d *= val;
        if ( d < 0. )
            return -dist;
        else
            return dist;
    }

    return dist;
}

float distance( float fx, float fy, float fz,
                                cxLine3dDesc &line ) {
    if ( line.type == line_normal ) {
        float tmp = line.m * (fx - line.fx) + line.n * (fy - line.fy) + line.p * (fz - line.fz);
        float scale = line.m * line.m;
        scale += line.n * line.n;
        scale += line.p * line.p;
        float t = tmp/scale;
        float xc = line.m * t + line.fx;
        float yc = line.n * t + line.fy;
        float zc = line.p * t + line.fz;

        float dx = (fx - xc)*(fx - xc);
        float dy = (fy - yc)*(fy - yc);
        float dz = (fz - zc)*(fz - zc);

        float dist = sqrt(dx+dy+dz);
      //  fprintf( stderr, "distance orig is %f t: %f loc: %f %f %f line: %f %f %f\n", dist, t,  fx,fy,fz, xc,yc,zc);

        return dist;
    } else { //other line
        float dist = fabs( fy - line.fy );
        return dist;
    }
}

float distance( float fx, float fy,
                                cxLineDesc &line ) {
    if ( line.type == line_normal )
        return distance( fx, fy, line.k, line.b );
    else if ( line.type == line_vertical ) {
        float dist = fabs( fx - line.fx );
        return dist;
    } else { //horizontal line
        float dist = fabs( fy - line.fy );
        return dist;
    }
}

float distance( float fx, float fy,
                float st_x, float st_y,
                float end_x, float end_y, int ltype ) {

    if ( ltype == line_normal ) {
        float k = ( end_y - st_y ) / ( end_x - st_x );
        float b = 0;
        return distance( fx, fy, k, b );
    } else if ( ltype == line_vertical ) {
        float dist = fabs( fx - st_x );
        return dist;
    } else { //horizontal line
        float dist = fabs( fy - st_y );
        return dist;
    }
}

float distance( float fx, float fy,
                float k, float b ) {

    float dist = 0.f;
    float _k = -1.f / k;
    float _b = fy - fx*_k; //kx + b = y; b = y - kx;
    float tx = 0.f;
    float ty = 0.f;
    float delx = 0.f;
    float dely = 0.f;

    intersection( k, b, _k, _b, tx, ty );

    delx = tx - fx;
    dely = ty - fy;
    delx *= delx;
    dely *= dely;
    dist = sqrt( delx + dely );
    return dist;
}

float distance( float fx, float fy,
                float k, float b, float &tx, float &ty ) {
    float dist = 0.f;
    float _k = -1.f / k;
    float _b = fy - fx*_k; //kx + b = y; b = y - kx;
    float delx = 0.f;
    float dely = 0.f;
    intersection( k, b, _k, _b, tx, ty );
    delx = tx - fx;
    dely = ty - fy;
    delx *= delx;
    dely *= dely;
    dist = sqrt( delx + dely );
    return dist;
}

void intersection( float k, float b, float _k, float _b, float &tx, float &ty ) {
    //kx+b = y;
    //k'x + b' = y;
    //k*k'x + k'*b = k'*y;
    //k*k'x + k*b' = k*y;
    //k'*b - k*b' = ( k'-k)*y;
    //y = (k'*b - k*b') / ( k'-k)
    ty = ( _k*b - k*_b ) / ( _k- k );
    tx = ( ty - b ) / k;
}

int getTanAngle( float tan ) {

    float k = tan;
    float tval[8] = { 0.198912367f, //11.25   0
                                0.668178638f, //33.75   1
                                1.496605763f, //56.25   2
                                5.027339492f, //78.75    3
                                -5.027339492f, //101.25    4
                                -1.496605763f, //123.75    5
                                -0.668178638f, //146.25    6
                                -0.198912367f, //168.75    7
                      };

    if ( k > 0. ) {
        int i = 0;
        while ( i < 4 ) {
            if ( k < tval[i] )
                break;
            i++;
        }
        //quadrand I
            return i;

    } else {
        int i = 4;
        while( i < 8 ) {
            if ( k < tval[i] )
                break;
            i++;
        }
        //quadrand II
        return i;

    }

}

//                    4
//            5              3
//        6                      2
//       7                         1
//      8             *            0
//        9                      15
//         10                 14
//            11           13
//                    12
int getTAngle( float dx, float dy ) {
    bool bfx = false;
    bool bfy = false;
    if ( isEqualF( dx, 0.f ) )
        bfx = true;
    if ( isEqualF( dy, 0.f ) )
        bfy = true;

    if ( bfx &&  bfy ) {
        fprintf( stderr, "vector distance is zero, only be a point!");
        return -1;
    } else if ( bfx ) { // vertical
        if ( dy > 0 )
            return 4;
        else
            return 12;
    } else if ( bfy ) { //horizontal
        if ( dx < 0 )
            return 8;
        else
            return 0;
    }

    float k = dy / dx;
    float tval[8] = { 0.198912367f, //11.25   0
                                0.668178638f, //33.75   1
                                1.496605763f, //56.25   2
                                5.027339492f, //78.75    3
                                -5.027339492f, //101.25    4
                                -1.496605763f, //123.75    5
                                -0.668178638f, //146.25    6
                                -0.198912367f, //168.75    7
                      };

    if ( k > 0. ) {
        int i = 0;
        while ( i < 4 ) {
            if ( k < tval[i] )
                break;
            i++;
        }
        if ( dx > 0. ) { //quadrand I
            return i;
        } else  { //quadrand III
            i += 8;
            return i;
        }
    } else {
        int i = 4;
        while( i < 8 ) {
            if ( k < tval[i] )
                break;
            i++;
        }
        if ( dx < 0. ) { //quadrand II
            return i;
        } else { //quadrand IV
            i += 8;
            if ( i == 16 )
                i = 0;
            return i;
        }
    }

}

void vector( cxVector3d &vector, cxPoint3F &st, cxPoint3F &ed ) {
    float dx = ed.fx -  st.fx;
    float dy = ed.fy - st.fy;
    float dz = ed.fz - st.fz;
    vector.length = sqrt( dx*dx + dy*dy + dz*dz );
    vector.directxy =getTAngle( dx, dy );
    vector.directyz =getTAngle( dy, dz );
    vector.directxz =getTAngle( dx, dz );
}

void vector( cxVector &vector, cxPointF &st, cxPointF &ed ) {
    float dx = ed.fx -  st.fx;
    float dy = ed.fy - st.fy;
    vector.length = sqrt( dx*dx + dy*dy );
    vector.direct =getTAngle( dx, dy );
}

void vector( cxLVector &lvector, cxPointF &st, cxPointF &ed ) {
    lvector.pts[0] = st;
    lvector.pts[1] = ed;
    vector( lvector.vector, st, ed );
}

float rotationf ( float angle0, float angle1 ) {
    float a = angle1 - angle0;
    return a;
}

int rotation ( int angle0, int angle1 ) {
    int a = angle1 - angle0;
    if ( a == 0 )
        return 0;
    if ( a > 0 ) {
        if ( a < 8 )
            return a;
        else {
            a -= 16;
            return a;
        }
    } else { // a < 0
        if ( a > -8 )
            return a;
        else {
            a += 16;
            return a;
        }
    }

}

void normalize3D( float *fval ) {
    float r = fval[0] * fval[0] + fval[1] * fval[1] + fval[2] * fval[2];
    r = sqrt( r );
    fval[0] /= r;
    fval[1] /= r;
    fval[2] /= r;
}

cxmath::cxmath()
{

}

