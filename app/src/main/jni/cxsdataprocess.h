#ifndef CXSDATAPROCESS_H
#define CXSDATAPROCESS_H

#include "cxsensordata.h"
enum {
    desc_null = 0,
    desc_normalize = 1,
    desc_zeroMean,
    desc_num
};

typedef struct data_describe {
    data_describe() {
        itype = 0;
    }
    void clear() {
        itype = 0;
    }
    double *params() {
        return  NULL;
    }
    int itype;
}data_describe;

typedef struct zmean_dsc : public data_describe{
    zmean_dsc() {
        mu = 0.;
        itype = desc_zeroMean;
    }
    void clear() {
        mu = 0.;
        itype = desc_null;
    }
    double *params() {
        return  &mu;
    }
    double mu;
} zmean_dsc;

typedef struct normal_dsc : public data_describe{
    normal_dsc() {
        min = 0.;
        max = 0.;
        module = 0.;
        itype = desc_normalize;
    }
    void clear() {
        min = 0.;
        max = 0.;
        module = 0.;
        itype = desc_null;
    }
    double *params() {
        return  val;
    }
    union {
        struct {
            double min;
            double max;
            double module;
        };

        double val[3];
    };

} normal_dsc;

normal_dsc* mallocNormalDesc();
zmean_dsc* mallocZMeanDesc();
data_describe* mallocDescribe( int mode );

typedef struct handle_sdata {
    handle_sdata() {
        nDesc = 0;
        desc = NULL;
    }

    void clear() {
        sdata.clear();
        for ( int i = 0; i < nDesc; i++ ) {
            desc[i]->clear();
            CXDFFREE( desc[i] );
        }
        CXDFFREE( desc );
        nDesc = 0;
    }

    void addDescribe( int mode ) {
        data_describe *newDesc = mallocDescribe( mode );
        if ( !newDesc )
            return;
        if ( !desc ) {
            desc = (data_describe**)malloc( sizeof(data_describe*) );
            desc[nDesc] = newDesc;
            nDesc = 1;
        } else {
            data_describe **tmp = desc;
            desc = (data_describe**)malloc( sizeof(data_describe*) * (nDesc+1));
            memcpy( desc, tmp, sizeof(data_describe*) * nDesc );
            desc[nDesc] = newDesc;
            CXDFFREE(tmp);
        }
    }

    bool isEmpty() {
        if ( nDesc > 0 && desc && sdata.vals && sdata.nVal )
            return true;
        return false;
    }

    sensor_data sdata;
    int nDesc;
    data_describe **desc;

}handle_sdata;

void normalize( double *data, int num, normal_dsc *dsc );
void zeroMean(  double *data, int num, zmean_dsc *dsc );
void processDescribe( handle_sdata *hdata );

class CXSDataProcess
{
public:
    CXSDataProcess();
};

#endif // CXSDATAPROCESS_H
