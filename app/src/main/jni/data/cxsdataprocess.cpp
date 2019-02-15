#include "cxsdataprocess.h"

normal_dsc* mallocNormalDesc() {
    normal_dsc *ptr = (normal_dsc*)malloc( sizeof(normal_dsc) );
    memset( ptr, 0, sizeof(normal_dsc) );
    ptr->itype = desc_normalize;
    return ptr;
}
zmean_dsc* mallocZMeanDesc() {
    zmean_dsc *ptr = (zmean_dsc*)malloc( sizeof(zmean_dsc) );
    memset( ptr, 0, sizeof(zmean_dsc) );
    ptr->itype = desc_zeroMean;
    return ptr;
}

data_describe* mallocDescribe( int mode ) {
    if ( mode > desc_null && mode < desc_num ) {
        switch( mode ) {
        case desc_normalize : {
            return mallocNormalDesc();
        }
        case desc_zeroMean : {
            return mallocZMeanDesc();
        }
        }//endswitch
    }
    return NULL;
}

void normalize(  double *data, int num, normal_dsc *dsc )
{
    double max = data[0];
    double min = data[0];
    double *val = data;
    double *limit = val + num;
    while ( val < limit ) {
        if (  max < *val )
            max = *val;
        if ( min > *val )
            min = *val;
        val++;
    }
    double d = 0.;
    if ( fabs(max) > fabs(min) )
        d = fabs(max);
    else
        d = fabs(min);

    val = data;
    while ( val < limit ) {
        *val = *val / d;
    }

    dsc->module = d;
    dsc->min = min;
    dsc->max = max;
}

void zeroMean( double *data, int num, zmean_dsc *dsc )
{
    double dval = 0.;
    double *val = data;
    double *limit = data + num;
    while ( val < limit ) {
        dval += *val;
    }

    dval /= num;
    val = data;
    while ( val < limit ) {
        *val = *val - dval;
    }

    dsc->mu = dval;
}

void processDescribe( handle_sdata *hdata )
{
    if ( !hdata->isEmpty() )
        return;
    for ( int i = 0 ; i < hdata->nDesc; i++ ) {
        data_describe *pDsc = hdata->desc[i];
        switch( pDsc->itype ) {
        case desc_normalize : {
            normal_dsc *normalDsc = (normal_dsc*)pDsc;
            normalize( hdata->sdata.vals, hdata->sdata.nVal, normalDsc);
            break;
        }
        case desc_zeroMean : {
            zmean_dsc *zMeanDsc = (zmean_dsc*)pDsc;
            zeroMean( hdata->sdata.vals, hdata->sdata.nVal, zMeanDsc);
            break;
        }
        default:
            break;
        }//enswitch
    }//endfor
}

CXSDataProcess::CXSDataProcess()
{

}

