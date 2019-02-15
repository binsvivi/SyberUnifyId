#include "cxsensordata.h"
#include "cxsdataprocess.h"

#include <stdio.h>

#ifdef CXDF_QT
#include "cxservice.h"
#endif

sample_map CXSensorSample::gSampleMap;
sample_buffer CXSensorSample::gSampleBuffer;
SampleSize CXSensorSample::gSampleSize;

static unsigned int m_sKID = 0;

unsigned int getSampleKeyID() {
    return m_sKID++;
}

const char* SensorName[CXDFSENSOR_NUM] = {
    "touch",
    "grav",
    "accs",
    "gyro",
    "magnetics"
};

const float SensorColor[CXDF_SENSOR_CHANNEL_NUM][4] = {
    { 1.f, 0.f, 0.f, 0.8f },
    { 0.f, 1.f, 0.f, 0.8f },
    { 0.f, 0.f, 1.f, 0.8f }
};

const float regionColor[4] = { 0.2, 0.2, 0.2, 0.6 };

const float SensorPointPix[2] = { 6.4f, 6.4f };
const float SensorAxisSCaleX[2] = { 0.95f, 4.f }; ////length scale, width



const char* sample_pathInfo = "/home/xizhi/UnifyID/ini/samples.info";


int getSensorType( char *chr ) {
    for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
        if ( !strcmp( chr, SensorName[i] ) )
            return i;
    }
    return -1;
}

int getSensorType( int bit ) {
    switch ( bit ) {
    case CXDF_SENSOR_TOUCH_BIT:
        return 0;
    case CXDF_SENSOR_GRAV_BIT:
        return 1;
    case CXDF_SENSOR_ACCS_BIT:
        return 2;
    case CXDF_SENSOR_GYRO_BIT:
        return 3;
    case CXDF_SENSOR_MAGNETICS_BIT:
        return 4;
    default:
        return -1;
    }
}

int64_t getint64TimeTag( const char *str ) {
    int64_t ret = 0;
    int64_t val = 0;
    int n = strlen(str);
    for ( int i = 0; i < n; i++ ) {
        val = str[i];
        if ( val < 48 || val > 57 )
            return -1;
        val -= 48;
        ret *= 10;
        ret += val;
    }
    return ret;
}

void convertint64TimeTagToString( int64_t tag, char *str ) {
    char chr = 0;
    int code = 0;
    int n = 0;
    while ( tag > 0 ) {
        code = tag%10;
        chr = code + 48;
        str[n++] = chr;
        tag /= 10;
    }

    int i = 0;
    int j = n-1;
    while( i < j ) {
        chr = str[i];
        str[i] = str[j];
        str[j] = chr;
        i++;
        j--;
    }
}

sensor_sample* newSensorSample() {
    sensor_sample* ss = (sensor_sample*) malloc( sizeof(sensor_sample) );
    if ( ss )
            memset( ss, 0, sizeof(sensor_sample ));
    return ss;
}

sensor_usample* newSensorUSample() {
    sensor_usample* ss = (sensor_usample*) malloc( sizeof(sensor_usample) );
    if ( ss )
            memset( ss, 0, sizeof(sensor_usample ));
    return ss;
}

void getGlobalSampleSize( SampleSize *gSize, sensor_sample *sample )
{
    bool bf = false;
    //time
    if ( sample->time.local.sizeT ) {
        if ( !gSize->m_Time.local.sizeT ) {
            gSize->m_Time.local.sizeT = sample->time.local.sizeT;
            gSize->m_Time.local.minT = sample->time.local.minT;
            gSize->m_Time.local.maxT = sample->time.local.maxT;
        } else {
            bf = false;
            if ( gSize->m_Time.local.minT > sample->time.local.minT ) {
                gSize->m_Time.local.minT = sample->time.local.minT;
                bf = true;
            }
            if ( gSize->m_Time.local.maxT < sample->time.local.maxT ) {
                gSize->m_Time.local.maxT = sample->time.local.maxT;
                bf = true;
            }
            if (  bf )
                gSize->m_Time.local.sizeT = gSize->m_Time.local.maxT - gSize->m_Time.local.minT ;

        }
    }
    //sensor size
    SenSorSize sensor;
    double *pVal = NULL;
    int c = 0;
    for ( int i = 0; i < SENSOR_NUM; i++ ) {
        if ( sample->sensors[i].isEmpty() )
            continue;
        sensor_data *sdata = &sample->sensors[i].sdata;
        sensor_TData *ptr = sdata->tdatas;
        sensor_TData *limit = ptr + sdata->nData;
        while( ptr < limit ) {
            pVal = ptr->val;
            c = 0;
            for ( int j = 0; j < ptr->size; j++ ) {
                if ( sensor.m_ChannelSize[c].size < 0 ) { //first time
                    sensor.m_ChannelSize[c].min = pVal[j];
                    sensor.m_ChannelSize[c].max = pVal[j];
                    sensor.m_ChannelSize[c].size = 0.;
                } else {
                    if ( sensor.m_ChannelSize[c].min > pVal[j] )
                        sensor.m_ChannelSize[c].min = pVal[j];
                    if ( sensor.m_ChannelSize[c].max < pVal[j] )
                        sensor.m_ChannelSize[c].max = pVal[j];
                }
                c++;
                if ( c == CXDF_SENSOR_CHANNEL_NUM )
                    c = 0;
            }//end for channel
            ptr++;
        } //endwhile for sensor sample

        SenSorSize *gsensor = &gSize->m_SensorSize[i];
        for ( int k = 0; k < CXDF_SENSOR_CHANNEL_NUM; k++ ) {
            if ( sensor.m_ChannelSize[k].size < 0 )
                continue;
            sensor.m_ChannelSize[k].size = sensor.m_ChannelSize[k].max - sensor.m_ChannelSize[k].min;
            bf = false;
            if ( gsensor->m_ChannelSize[k].min > sensor.m_ChannelSize[k].min ) {
                gsensor->m_ChannelSize[k].min = sensor.m_ChannelSize[k].min;
                bf = true;
            }
            if ( gsensor->m_ChannelSize[k].max < sensor.m_ChannelSize[k].max ) {
                gsensor->m_ChannelSize[k].max = sensor.m_ChannelSize[k].max;
                bf = true;
            }
            if ( bf ) {
                gsensor->m_ChannelSize[k].size = gsensor->m_ChannelSize[k].max - gsensor->m_ChannelSize[k].min;
            }
        }//update gsensor

        sensor.clear();
    }//endfor sample sensor

    for ( int i = 0; i < SENSOR_NUM; i++ )
        gSize->m_SensorSize[i].computeSensorSize();
}


CXSensorSample::CXSensorSample()
{
    m_path = NULL;
}

CXSensorSample::~CXSensorSample()
{
    clear();
}

void CXSensorSample::clear()
{
    CXDFFREE( m_path );
    m_task.clear();
    m_utask.clear();
}

int CXSensorSample::readSensor( const char *path )
{
    FILE *fp = fopen( path, "r" );
    if ( !fp )
        return -1;
    char buffer[1024] = { 0 };

    int length = 0;
    int tag = 0;
    int bf = 0;
    int type = 0;
    int step = 0;
    int bit = 0;
    int nbit = 0;

    while ( fscanf( fp, "%s", buffer ) != EOF ) {
       // fprintf( stderr, "read sample ~~~~~~~~~%s~\n", buffer);
        length = strlen( buffer );
        if ( length == 1 && buffer[0] == '<' ) { //start
            bf++;
            step++;
        } else if ( length == 1 && buffer[0] == '>' ) { //end
            bf--;
            if ( step ==2 ) {
                tag = 0;
                step = 0;
                nbit = 0;
            }
        } else {
            if ( step == 1 && bf && nbit >=0 ) { //read sensor type tag
                type = getSensorType( buffer );
                if ( type < 0 ) {
                    tag = 0;
                    nbit = -1;
                    fprintf( stderr, "readSensor %s, unknow type %s ", path, buffer );
                } else {
                    switch ( type ) {
                    case SENSOR_TOUCH:
                        bit = CXDF_SENSOR_TOUCH_BIT;
                        break;
                    case SENSOR_GRAV:
                        bit = CXDF_SENSOR_GRAV_BIT;
                        break;
                    case SENSOR_ACCS:
                        bit = CXDF_SENSOR_ACCS_BIT;
                        break;
                    case SENSOR_GYRO:
                        bit = CXDF_SENSOR_GYRO_BIT;
                        break;
                    case SENSOR_MAGN:
                        bit = CXDF_SENSOR_MAGNETICS_BIT;
                        break;
                    default:
                        break;
                    }
                    bit <<= nbit*CXDF_SENSOR_BITSPAN;
                    tag |= bit;
                    nbit++;
                }

            } else if ( step == 2 && bf && nbit > 0 ) { //read file
                    readFileList( buffer, tag );
                 //  fprintf( stderr, "read sample ~~~~~~123~~~~\n");
            }
        }

        memset( buffer, 0, 1024 );
    }
   // fprintf( stderr, "read sample ~~~~~~~~~~");

    fclose( fp );

    length = strlen(path);
    m_path = (char*)malloc( sizeof(char) * (  length + 1));
    memcpy( m_path, path, length );
    m_path[length] = 0;

    //ceshi
//    for ( int i = 0; i < m_task.nSample; i++ ) {
//        if ( m_task.samples[i] != gSampleMap.map[i].sample )
//            fprintf( stderr, "error" );
//    }

    return 0;
}

int CXSensorSample::readFileList(const char *filePath, int mask )
{
  //  fprintf( stderr, "read sample file list %s \n", filePath );
    FILE *fp = fopen( filePath, "r" );
    if ( !fp )
        return -1;
   // fprintf( stderr, "read sample file list opened %s \n", filePath );
    char buffer[1024] = { 0 };
    char keyStr[CXDF_SENSOR_KEY_LENGTH+1] = {0};
    int sum = 0;
    int length = 0;
    FILE *target = NULL;
    char *ptr = NULL;

    while ( fscanf( fp, "%s", buffer ) != EOF ) {
        target = fopen( buffer, "r" );
        if ( target ) {
            sum++;
            fclose( target);
        }
        memset( buffer, 0, 1024);
    }
    fprintf( stderr, "read sample file list opened sum = %d \n", sum );

    int type[CXDFSENSOR_NUM] = {0};
    int nType = 0;
    unsigned char bit = mask;
    bit &= CXDF_SENSOR_BITMASK;
    while( bit ) {
        type[nType] = bit&CXDF_SENSOR_BITMASK;
        nType++;
        mask >>= CXDF_SENSOR_BITSPAN;
        bit = mask;
        bit &= CXDF_SENSOR_BITMASK;
    }

    if ( !sum || !nType ) {
     //   fprintf( stderr, "read sample ~~~~~~~~~~");
        fclose(fp);
        return -1;
    }

    fseek( fp, 0, SEEK_SET );
    while ( fscanf( fp, "%s", buffer ) != EOF ) {
        target = fopen( buffer, "r" );
        if ( !target )
            continue;
        //fprintf( stderr, "read sample ~~~~~~~~%s~~\n", buffer);

        length = strlen( buffer );
        ptr = buffer + length;
        while ( *ptr != '.' )
            ptr--;
        ptr -= CXDF_SENSOR_KEY_LENGTH;
        memcpy( keyStr, ptr, CXDF_SENSOR_KEY_LENGTH );
        sensor_sample *sample = getSample( keyStr );
        if ( !sample ) {
            fprintf( stderr, "read sample null ~~~~~~~~~~");
            fclose( target );
            fclose( fp );
            return -1;
        }

        m_task.insert( sample );
        readSampleFromFile( target, type, nType, sample );
        checkSample( sample );
       // fprintf( stderr, "read sample ~~~~~~~~~~\n");

        fclose( target );
        memset( buffer, 0, 1024);
    }
  //  fprintf( stderr, "read sample ~~~~~~~5555~~~\n");
    fclose( fp );
    return 0;
}

int CXSensorSample::readSampleFromFile( FILE *fp, int *type, int nType, sensor_sample *sample )
{
    //fprintf( stderr, "read sample From File~~~~~~~~~~\n");
    int index[CXDFSENSOR_NUM] = {0};
    for ( int i = 0; i < nType; i++ ) {
        index[i] = getSensorType( type[i] );
    }
    int ret = 0;
    int i = 0;
    int j = 0;
    int64_t timeTag = 0;
    double val[3] = { 0., 0., 0. };
    char tag[20] = { 0 };
    fseek( fp, 0, SEEK_SET );
    gSampleBuffer.reset();
    memset( tag, 0, 20);
    while ( fscanf( fp, "%s", tag ) != EOF ) {
        timeTag = getint64TimeTag( tag );
       // fprintf( stderr, "read sample ~~~~~~~%s~~~\n", tag);

        memset( tag, 0, 20 );
        beginWriteTimeTag( index[i], timeTag );
//        if ( index[i] > SENSOR_TOUCH ) {
            memset( val, 0, sizeof(double) * 3);
       //     fprintf( stderr, "read sample ~~~~~~~22222222~~%d~\n",j);
            while ( fscanf( fp, "%lf", &val[j] ) != EOF ) {
          //      fprintf( stderr, "read sample ~~~~~~~%lf~~~\n", val[j]);
                j++;
                if ( j == 3 )
                    break;
            }
            if ( j !=3  ) {
                fprintf( stderr, "read sample j=! 3 ~~~~~~~~~~");
                ret = -1;
                break;
            }
           // fprintf( stderr, "read sample ~~~~~~~444~~%d~\n",j);

            writeVal( index[i], val, 3 );
            endWrite( index[i], 3 );

            i++;
            j = 0;
            if ( i == nType )
                i = 0;

//        }
    }

    //fprintf( stderr, "read sample ~~~~~~~~ret %d~~\n", ret);
    if ( !ret )
        writeToSensorSample( sample );



    //ceshi
//    handle_sdata hdata;
//    normalize( &sample->sensors[1].sdata, &hdata );

//    const char* filePath = "/home/xizhi/sensorceshi.txt";
//    printSampleToFile( sample, filePath );

    return ret;
}

void CXSensorSample::beginWriteTimeTag( int nid, int64_t tag )
{
    sensor_buffer *sbuffer = &gSampleBuffer.sbuffer[nid];
    sbuffer->tdatas[sbuffer->nData].timeTag = tag;
    sbuffer->tdatas[sbuffer->nData].val = sbuffer->last;
}

void CXSensorSample::writeVal( int nid, double *vals, int nval )
{
    sensor_buffer *sbuffer = &gSampleBuffer.sbuffer[nid];
    double *ptr = sbuffer->last;
    memcpy( ptr, vals, sizeof(double) * nval );
    sbuffer->last += nval;
    sbuffer->nVal += nval;
}

void CXSensorSample::endWrite( int nid, int nval )
{
    sensor_buffer *sbuffer = &gSampleBuffer.sbuffer[nid];
    sbuffer->tdatas[sbuffer->nData++].size = nval;
}

void CXSensorSample::writeToSensorSample( sensor_sample *sample )
{
    for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
        sensor_buffer *sbuffer = &gSampleBuffer.sbuffer[i];
        if ( !sbuffer->nVal )
            continue;
   //     fprintf( stderr, "read sample ~~~~%d~~~nData~~~\n",i);

        sensor_data *sdata = &sample->sensors[i].sdata;
        //sensor value
        int size = sizeof(double) * sbuffer->nVal;
        sdata->vals = (double*)malloc( size );
        memcpy( sdata->vals, sbuffer->vals, size );
        //sensor sample data
        size = sizeof(sensor_TData) * sbuffer->nData;
        sdata->tdatas = (sensor_TData*)malloc( size );
        memcpy( sdata->tdatas, sbuffer->tdatas, size );
        //frame number
        sdata->nData = sbuffer->nData;
        sdata->nVal = sbuffer->nVal;
        //reset buffer
        double *ptr = sdata->vals;
        for ( int j = 0; j < sdata->nData; j++ ) {
            sdata->tdatas[j].val = ptr;
            ptr += sdata->tdatas[j].size;
        }
    }
    checkTouchSensor( &sample->sensors[SENSOR_TOUCH].sdata );
}

void CXSensorSample::checkTouchSensor( sensor_data *touch )
{
    if ( touch->isEmpty() )
        return;
    sensor_TData *ptr = touch->tdatas;
    sensor_TData *end = ptr + touch->nData - 2;
    int num = touch->nData;
    while( num > 2 ) {
        if ( isEqualF( ptr[0].val[1], ptr[1].val[1]) &&
                isEqualF( ptr[0].val[1], ptr[1].val[1])     ) {
            ptr++;
            num--;
        } else
            break;
    }
    while( num > 2 ) {
        if ( isEqualF( end[0].val[1], end[1].val[1]) &&
                isEqualF( end[0].val[1], end[1].val[1])     ) {
            end--;
            num--;
        } else
            break;
    }

    if ( num != touch->nData ) {
        int size = sizeof(sensor_TData) * num;
        sensor_TData *buffer = (sensor_TData*)malloc( size );
        memcpy( buffer, ptr, size );
        CXDFFREE( touch->tdatas );
        touch->tdatas = buffer;
        touch->nData = num;
    }
}

void CXSensorSample::sortTask()
{
    int min = 0;
    sensor_sample **s = m_task.samples;
    sensor_sample *tmp = NULL;
    for ( int i = 0; i < m_task.nSample; i++ ) {
        min = i;
        for ( int j = i + 1; j < m_task.nSample; j++ ) {
            if ( s[min]->key.i64Key > s[j]->key.i64Key )
                min = j;
        }
        if ( min != i ) {
            tmp = s[i];
            s[i] = s[min];
            s[min] = tmp;
        }
    }
}

void CXSensorSample::getUnionSample()
{
    if ( !m_task.nSample || !m_task.samples )
        return;
    m_utask.usamples = (sensor_usample**) malloc( sizeof(void*) * m_task.nSample );
    for ( int i = 0, j = 0; i < m_task.nSample; i++ ) {
        sensor_sample *sample = m_task.samples[i];
        sensor_usample *usample = getUnionSample( sample );
        if ( usample )
            m_utask.usamples[j++] = usample;
    }
}

sensor_usample* CXSensorSample::getUnionSample( sensor_sample *sample )
{
    return NULL;
}

void CXSensorSample::checkSample( sensor_sample *sample )
{
    sample->checkTime();
    if ( sample->sensors[0].sdata.isEmpty() )
        sample->locateAccsTime();
    else
        sample->locateTouchTime();
    getGlobalSampleSize( &gSampleSize, sample );

}

//static function

 sample_pair* CXSensorSample::findPair( int key )
 {
     if ( key < 0 )
         return NULL;

     sample_pair *pair = gSampleMap.map;
     sample_pair *limit = pair + gSampleMap.last;
     while( pair < limit ) {
         if ( !pair->isEmpty() && pair->getKey() == key )
             return pair;
         pair++;
     }
     return NULL;
 }

sensor_sample* CXSensorSample::findSample( const char *keyStr )
{
    sample_pair *pair = gSampleMap.map;
    sample_pair *limit = pair + gSampleMap.last;
    while ( pair < limit ) {
        sensor_sample *sample = pair->getSample();
        if ( sample && !strcmp( keyStr, sample->key.getKeyStr()) ) {
            return sample;
        }
        pair++;
    }
      fprintf( stderr, "CXSensorSample::findSample ~~~~~~~%s~~~\n", keyStr);

    return NULL;
}

sensor_usample* CXSensorSample::findUSample( const char *keyStr, int key )
{
    sample_pair *pair = gSampleMap.map;
    sample_pair *limit = pair + gSampleMap.last;
    while ( pair < limit ) {
        sensor_usample *usample = pair->getUionSample();
        if ( usample &&
             ( ( keyStr && !strcmp( keyStr, pair->getKeyStr()) )||
                pair->getKey() == key ) ) {
            return usample;
        }
        pair++;
    }
    return NULL;
}

sensor_sample* CXSensorSample::getSample( const char *keyStr )
{
    sensor_sample *sample = findSample( keyStr );
    if ( !sample ) {
        //new sample and insert into map
        sample = newSensorSample();
        if ( !sample )
            return  sample;
        sample->key.setKeyStr( keyStr );
        addSample( sample );
    }
    return sample;
}

sensor_usample* CXSensorSample::getUSample( const char *keyStr, int key )
{
    sensor_usample *usample = findUSample( keyStr, key );
    if ( !usample ) {
        //new sample and insert into map
        usample = newSensorUSample();
        if ( !usample )
            return  usample;
        if ( keyStr )
            usample->key.setKeyStr(keyStr);
        addUSample( usample );
    }
    return usample;
}

int CXSensorSample::addSample( sensor_sample *sample )
{
    if ( gSampleMap.num >=CXDFSENSOR_MAX_NUM_SAMPLES &&
            gSampleMap.last == gSampleMap.num )
        return -1;

    sample_pair *pair = NULL;
    if ( gSampleMap.num < gSampleMap.last ) {
        int i = 0;
        while ( i < gSampleMap.last ) {
            if (  gSampleMap.map[i].isEmpty() ) {
                gSampleMap.map[i].clear();
                pair = &(gSampleMap.map[i] );
                break;
            }
        }
    }

    if ( !pair ) {
        pair = &(gSampleMap.map[gSampleMap.num] );
        gSampleMap.last++;
        gSampleMap.num = gSampleMap.last;
    }

    pair->setSample( sample );
    pair->setKey(getSampleKeyID());
    sample->key.setKey(pair->getKey());
    pair->setKeyStr( sample->key.getKeyStr());

#ifdef CXDF_QT
    getGlobalService()->addSample( pair );
#endif

    return 0;
}

int CXSensorSample::addUSample( sensor_usample *usample )
{
    if ( gSampleMap.num >=CXDFSENSOR_MAX_NUM_SAMPLES &&
            gSampleMap.last == gSampleMap.num )
        return -1;

    sample_pair *pair = NULL;
    if ( gSampleMap.num < gSampleMap.last ) {
        int i = 0;
        while ( i < gSampleMap.last ) {
            if ( gSampleMap.map[i].isEmpty() ) {
                pair = &(gSampleMap.map[i] );
                break;
            }
        }
    }

    if ( !pair ) {
        pair = &(gSampleMap.map[gSampleMap.num] );
        gSampleMap.last++;
        gSampleMap.num = gSampleMap.last;
    }

    pair->setUnionSample( usample );
    pair->setKey(getSampleKeyID());
    usample->key.setKey(pair->getKey());
    pair->setKeyStr( usample->key.getKeyStr() );

#ifdef CXDF_QT
    getGlobalService()->addSample( pair );
#endif

    return 0;
}

int CXSensorSample::printSampleToFile( sensor_sample *sample, const char *path )
{
    FILE *fp = fopen( path, "w" );
    if ( !fp || !sample )
        return -1;

    char timTag[20] = { 0 };
    for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
        sensor_data *sdata = &sample->sensors[i].sdata;
        if ( !sdata->nData || !sdata->vals )
            continue;
        switch( i ) {
        case SENSOR_TOUCH :
            fprintf( fp, "< touch >\n" );
            break;
        case SENSOR_GRAV :
            fprintf( fp, "< grav >\n" );
            break;
        case SENSOR_ACCS :
            fprintf( fp, "< accs >\n" );
            break;
        case SENSOR_GYRO :
            fprintf( fp, "< gyro >\n" );
            break;
        }

        sensor_TData *fras = sdata->tdatas;
        for ( int j = 0; j < sdata->nData; j++ ) {
            memset( timTag, 0, 20 );
            convertint64TimeTagToString( fras[j].timeTag, timTag );
            fprintf( fp, "%s\n", timTag );
            for ( int n = 0; n < fras[j].size; n++ ) {
                fprintf( fp, "%lf\n", fras[j].val[n] );
            }
        }
        fprintf( fp, "\n" );
    }

    fclose( fp );
    return 0;
}

void interSensorPoint( sensor_TData *ptr, int64_t t, float *val ) {
    float dt = ptr[1].timeTag - ptr[0].timeTag;
    float a = (float)( t - ptr[0].timeTag ) / dt;
    float b = 1.f - a;
    val[0] = ptr[1].val[0] * a + ptr[0].val[0] * b;
    val[1] = ptr[1].val[1] * a + ptr[0].val[1] * b;
    val[2] = ptr[1].val[2] * a + ptr[0].val[2] * b;
}

void accsSubGrav( sensor_sample *sample ) {
    int n = sample->sensors[SENSOR_TOUCH].sdata.nData - 1;
    int64_t t0 = sample->sensors[SENSOR_TOUCH].sdata.tdatas[0].timeTag;
    int64_t t1 = sample->sensors[SENSOR_TOUCH].sdata.tdatas[n].timeTag;
    int64_t t = 0.;

    sensor_TData *accDatas = sample->sensors[SENSOR_ACCS].sdata.tdatas;
    int nAcc = sample->sensors[SENSOR_ACCS].sdata.nData;

    sensor_TData *gravDatas = sample->sensors[SENSOR_GRAV].sdata.tdatas;
    int nGrav = sample->sensors[SENSOR_GRAV].sdata.nData;

    sensor_TData *pri = NULL;
    sensor_TData *ptr = accDatas;
    sensor_TData *next = accDatas + 1;
    sensor_TData *limit = ptr + nAcc;
    sensor_TData *dst = gravDatas;
    sensor_TData *dlimit = dst + nGrav;

    bool bf = false;
    bool find  = false;
    float fval[3] = { 0.f, 0.f, 0.f };
    while ( ptr < limit && ptr) {
        bf = false;
        if ( pri && pri->timeTag >= t0 && pri->timeTag <= t1 )
            bf = true;
        else if ( ptr && ptr->timeTag >= t0 && ptr->timeTag <= t1 )
            bf = true;
        else if ( next && next->timeTag >= t0 && next->timeTag <= t1 )
            bf = true;
        if ( !bf && find )
            break;

        if ( bf ) {
            find = true;
            t = ptr->timeTag;
            while( dst < dlimit && dst->timeTag < t ) {
                dst++;
            }
            if ( dst == dlimit )
                break;

            if ( dst == gravDatas ||
                 dst->timeTag == ptr->timeTag ) {
                ptr->val[0] -= dst->val[0];
                ptr->val[1] -= dst->val[1];
                ptr->val[2] -= dst->val[2];
            } else {
                interSensorPoint( dst-1, t, fval );
                ptr->val[0] -= fval[0];
                ptr->val[1] -= fval[1];
                ptr->val[2] -= fval[2];
            }
        }

        pri = ptr;
        ptr = next;
        next++;
        if ( next == limit )
            next = NULL;
    }

}
