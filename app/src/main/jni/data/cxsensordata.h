#ifndef CXSENSORDATA_H
#define CXSENSORDATA_H

#include "cxbase.h"

#define CXDFSENSOR_MAX_NUM_SAMPLES      300
#define CXDFSENSOR_NUM                                  5

#define CXDF_SENSOR_TOUCH_BIT                   0x01
#define CXDF_SENSOR_GRAV_BIT                      0x02
#define CXDF_SENSOR_ACCS_BIT                       0x04
#define CXDF_SENSOR_GYRO_BIT                      0x08
#define CXDF_SENSOR_MAGNETICS_BIT                      0x0a

#define CXDF_SENSOR_BITSPAN                         4
#define CXDF_SENSOR_BITMASK                         0x0f
#define CXDF_SENSOR_KEY_LENGTH                  13
#define CXDF_SENSOR_CHANNEL_NUM             3

extern const char* sample_pathInfo;
extern const char* SensorName[CXDFSENSOR_NUM];
extern const float SensorColor[CXDF_SENSOR_CHANNEL_NUM][4];
extern const float SensorPointPix[2];
extern const float SensorAxisSCaleX[2]; //length scale, width
extern const float regionColor[4];

enum PaintType{
    panit_point = 0,
    paint_curve,
    paint_axis,
    paint_region,
};

typedef struct SenSorSize {
    SenSorSize() {}
    void clear() {
        for ( int i = 0; i < CXDF_SENSOR_CHANNEL_NUM; i++ )
            m_ChannelSize[i].clear();
        m_gSize.clear();
        m_Time.clear();
    }
    void computeSensorSize() {
        for ( int i = 0; i < CXDF_SENSOR_CHANNEL_NUM; i++ ) {
            if ( m_ChannelSize[i].size < 0 )
                continue;
            if ( m_gSize.size < 0 ) {
                m_gSize.min = m_ChannelSize[i].min;
                m_gSize.max = m_ChannelSize[i].max;
                m_gSize.size = m_gSize.max - m_gSize.min;
            } else {
                bool bf = false;
                if ( m_gSize.min > m_ChannelSize[i].min ) {
                    m_gSize.min = m_ChannelSize[i].min;
                    bf = true;
                }

                if ( m_gSize.max < m_ChannelSize[i].max ) {
                    m_gSize.max = m_ChannelSize[i].max;
                    bf = true;
                }

                if ( bf )
                    m_gSize.size = m_gSize.max - m_gSize.min;
            }
        }
    }

    SCSize m_ChannelSize[CXDF_SENSOR_CHANNEL_NUM];
    SCSize m_gSize;
    SCTimeSize m_Time;
}SenSorSize;

typedef struct SampleSize {
    void clear() {
        for ( int i = 0; i < SENSOR_NUM; i++ )
            m_SensorSize[i].clear();
        m_Time.clear();
    }

    SenSorSize m_SensorSize[SENSOR_NUM];
    SCTimeSize m_Time;
}SampleSize;

//sensor's  data
typedef struct sensor_TData{
    sensor_TData() {
        timeTag = 0;
        local_time = 0;
        val = NULL;
        size = 0;
    }
    void clear() {
        timeTag = 0;
        val = NULL;
        size = 0;
    }

    int64_t timeTag; //时间戳
    int64_t local_time; //相对时间
    double *val; //指向数据数组
    int size; //数组维数
} sensor_TData;

typedef struct sensor_data {
    sensor_data() {
        tdatas = NULL;
        vals = NULL;
        nData = 0;
        nVal = 0;
    }
    void clear() {
        CXDFFREE( tdatas );
        CXDFFREE( vals );
        nData = 0;
        nVal = 0;
    }
    bool isEmpty() {
        if ( !nData || !nVal || !vals || !tdatas )
            return true;
        return false;
    }
    void copy( sensor_data *target ) {
        clear();
        nData = target->nData;
        nVal = target->nVal;
        int size = sizeof( sensor_TData ) * nData;
        tdatas = ( sensor_TData* )malloc( size );
        memcpy( tdatas, target->tdatas, size );
        size = sizeof( double ) * nVal;
        vals = (double*)malloc( size );
        memcpy( vals, target->vals, size );
        double *ptr = vals;
        for ( int i = 0; i < nData; i++ ) {
            tdatas[i].val = ptr;
            ptr += tdatas[i].size;
        }
    }
    void getTimeSize( int64_t &minT, int64_t &maxT ) {
        sensor_TData *td = tdatas;
        sensor_TData *limit = td + nData - 1;
        minT = td->timeTag;
        maxT = limit->timeTag;
    }
    void getLocalTimeSize( int64_t &minT, int64_t &maxT ) {
        sensor_TData *td = tdatas;
        sensor_TData *limit = td + nData - 1;
        minT = td->local_time;
        maxT = limit->local_time;
    }
    void getChannelSize( double &min0, double &max0,
                                            double &min1, double &max1,
                                            double &min2, double &max2 ) {
        min0 = vals[0]; max0 = vals[0];
        min1 = vals[1]; max1 = vals[1];
        min2 = vals[2]; max2 = vals[2];
        double *dval = vals + 3;
        double *limit = vals + nVal;
        int mode = 0;
        while ( dval < limit ) {
            switch ( mode ) {
            case 0 : {
                if ( min0 > *dval )
                    min0 = *dval;
                if ( max0 < *dval )
                    max0 = *dval;
                break;
            }
            case 1 : {
                if ( min1 > *dval )
                    min1 = *dval;
                if ( max1 < *dval )
                    max1 = *dval;
                break;
            }
            case 2 : {
                if ( min2 > *dval )
                    min2 = *dval;
                if ( max2 < *dval )
                    max2 = *dval;
                break;
            }
            }//endswitch

            mode++;
            dval++;
            if ( mode == 3 )
                mode = 0;
        }//endwhile
    }

    void locateTime( int64_t T ) {
        for ( int i = 0; i < nData; i++ ) {
            tdatas[i].local_time = tdatas[i].timeTag - T;
         //   fprintf( stderr, "local time is %d \n",  tdatas[i].local_time);
        }
    }

    sensor_TData *tdatas;
    double *vals;
    int nData;
    int nVal;
}sensor_data;

//path : sensor data file path
//itype : bits of reserve sensor type
//sensor sample info
typedef struct sdata_Info {
    sdata_Info() {
        path = NULL;
        itype = 0;
    }
    void clear() {
        CXDFFREE( path );
        itype = 0;
    }
    char *path; //传感器数据文件路径
    int itype; //传感器数据类型
}sdata_Info;

//num : sizeof data
//itype : raw sensor data type
//datas : data
//info : data file source, cleared outsides
//sensor data packet
typedef struct sdata_packet {
    sdata_packet() {
        stype = -1;
        info = NULL;
    }

    void clear() {
        sdata.clear();
        stype = -1;
        info = NULL;
    }

    bool isEmpty() { return sdata.isEmpty(); }
    sensor_data sdata;//sensor's data
    int stype; //sensor's type
    sdata_Info *info;
} sdata_packet;

//sample keys :
//int key: sample int id
//str key: sample's resouce file str no key
typedef struct sample_key {
    enum {
        CXEN_SENSOR_KEY_LENGTH  = 13,
        CXEN_SENSOR_KEY_SIZE
    };
    sample_key() {
        key = -1;
        keyBit = 0;
        keyStr[CXDF_SENSOR_KEY_LENGTH] = 0;
    }
    void clear() {
        key = -1;
        keyBit = 0;
        keyStr[CXDF_SENSOR_KEY_LENGTH] = 0;
    }

    void setKey( int ikey ) {
        if ( ikey < 0 )
            return;
        keyBit |= 0x01;
        key = ikey;
    }

    void setKeyStr( const char *str ) {
        if ( !str )
            return;
        keyBit |= 0x10;
        memcpy( keyStr, str, CXDF_SENSOR_KEY_LENGTH );
        atoiKey();
    }

    int getKey() {
        if ( keyBit&0x01 )
            return key;
        return -1;
    }

    char* getKeyStr() {
        if ( keyBit&0x10 )
            return keyStr;
        return NULL;
    }

    void atoiKey() {
        int64_t w = CXEN_SENSOR_KEY_LENGTH - 1;
        w = pow( 10, (double)w );
        i64Key = 0;
        int64_t tmp = 0;
        for ( int i = 0; i < CXEN_SENSOR_KEY_LENGTH; i++ ) {
            tmp = (int)keyStr[i] - 48;
            if ( tmp > 0 ) {
                tmp *= w;
                i64Key += tmp;
            }
            w /= 10;
        }
    }

    int key;
    char keyStr[CXDF_SENSOR_KEY_LENGTH+1];
    char keyBit;
    int64_t i64Key;
}sample_key;

//batch of all sensor samples
//samples: all sensor type raw data
//numInfo: number of data source
typedef struct sensor_sample{
    sensor_sample() {
        infos = NULL;
        numInfo = 0;
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
            sensors[0].stype = i;
        }
    }

    void clear() {
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ )
            sensors[i].clear();
        for ( int i = 0; i < numInfo; i++ ) {
            infos[i]->clear();
            free( infos[i] );
        }

        numInfo = 0;
    }

    void checkTime() {
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
            if ( sensors[i].sdata.isEmpty() )
                continue;
            if ( time.sizeT == 0 ) {
                time.minT = sensors[i].sdata.tdatas[0].timeTag;
                time.maxT = sensors[i].sdata.tdatas[sensors[i].sdata.nData-1].timeTag;
                time.sizeT = time.maxT - time.minT;
            } else {
                int64_t min = sensors[i].sdata.tdatas[0].timeTag;
                int64_t max = sensors[i].sdata.tdatas[sensors[i].sdata.nData-1].timeTag;
                if ( time.minT > min )
                    time.minT = min;
                if ( time.maxT < max )
                    time.maxT = max;
            }
        }
        time.sizeT = time.maxT - time.minT;
    }

    void locateTime() {
        time.local.baseTime = time.minT;
        time.local.sizeT = time.sizeT;
        time.local.minT = 0;
        time.local.maxT = time.sizeT;

        for ( int i = 0; i < CXDFSENSOR_NUM; i++ )
            sensors[i].sdata.locateTime( time.local.baseTime );
    }

    void locateTime( int64_t T ) {
        time.local.baseTime = T;
        time.local.sizeT = time.sizeT;
        time.local.minT = time.minT - T;
        time.local.maxT = time.maxT - T;
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ )
            sensors[i].sdata.locateTime( T );
    }

    void locateTouchTime() {
        if ( sensors[0].sdata.isEmpty() )
            return;
        locateTime( sensors[SENSOR_TOUCH].sdata.tdatas[0].timeTag );
    }

    void locateAccsTime() {
        if ( sensors[SENSOR_ACCS].sdata.isEmpty() )
            return;
        locateTime( sensors[SENSOR_ACCS].sdata.tdatas[0].timeTag );
    }

    bool isEmpty() {
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
            if ( sensors[i].sdata.nData < 1 )
                return true;
        }
        return false;
    }

    sdata_packet sensors[CXDFSENSOR_NUM]; //reserve all sensor type sample raw data
    sdata_Info **infos; //raw data's resource
    int numInfo; //number of raw data resource
    sample_key key;
    SCTimeSize time;

}sensor_sample;

//sensor data project to 相同的时间
typedef struct sensor_UTData { //sensor union time data
    sensor_UTData() {
        memset( val, 0, sizeof(double) * CXDFSENSOR_NUM );
        memset( size, 0, sizeof(int) * CXDFSENSOR_NUM );
        timeTag = 0;
    }

    void clear() {
        memset( val, 0, sizeof(double) * CXDFSENSOR_NUM );
        memset( size, 0, sizeof(int) * CXDFSENSOR_NUM );
        timeTag = 0;
    }

    int64_t timeTag; //时间戳
    double *val[CXDFSENSOR_NUM]; //指向数据数组
    int size[CXDFSENSOR_NUM];
}sensor_UTData;

typedef struct sensor_usample {
    sensor_usample() {
        datas = NULL;
        nData = 0;
        memset( vals, 0, sizeof(double*) * CXDFSENSOR_NUM );
        memset( nVal, 0, sizeof(int*) * CXDFSENSOR_NUM );
        source = NULL;
        memset( sensors, 0, sizeof(sdata_packet) *CXDFSENSOR_NUM );
    }
    void clear() {
        CXDFFREE( datas );
        nData = 0;
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
            CXDFFREE( vals[i] );
            nVal[i] = 0;
            CXDFFREE(sensors[i].sdata.tdatas );
        }
        memset( sensors, 0, sizeof(sdata_packet) *CXDFSENSOR_NUM );
        source = NULL;
    }

    void SensorDataCast() {
        sdata_packet *ptr = sensors;
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
            ptr->stype = i;
            ptr->info = NULL;
            ptr->sdata.nVal = nVal[i];
            ptr->sdata.vals = vals[i];
            ptr->sdata.nData = nData;
            if ( ptr->sdata.vals && nData ) {
                ptr->sdata.tdatas = (sensor_TData*)malloc(sizeof(sensor_TData)*nData );
                for ( int j = 0; j < nData; j++ ) {
                    ptr->sdata.tdatas[j].timeTag = datas[j].timeTag;
                    ptr->sdata.tdatas[j].size = datas[j].size[i];
                    ptr->sdata.tdatas[j].val = datas[j].val[i];
                }
            }

            ptr++;
        }
    }

    sensor_UTData *datas;
    int nData;
    double *vals[CXDFSENSOR_NUM]; //指向数据数组
    double nVal[CXDFSENSOR_NUM]; //指向数据数组

    sdata_packet sensors[CXDFSENSOR_NUM];

    sample_key key;
    sensor_sample *source;
}sensor_usample;

sensor_sample* newSensorSample();

//////////////////////////////////// batch of sample task ////////////////////////////////////
typedef struct  CXSampleTask {
    CXSampleTask() {
        samples = NULL;
        nSample = 0;
    }

    void clear() {
        //all sample managed by sample_map gSampleMap
//        for ( int i = 0; i < nSample; i++ ) {
//            samples[i]->clear();
//            free( samples[i] );
//        }
        CXDFFREE( samples );
        nSample = 0;
    }

    void insert( sensor_sample *np ) {
        if ( !samples ) {
            samples = ( sensor_sample** )malloc(sizeof(void*) );
            samples[0] = np;
            nSample = 1;
        } else {
            bool bf = false;
            for ( int i = 0; i < nSample; i++ ) {
                if ( samples[i] == np ) {
                    bf = true;
                    break;
                }
            }
            if ( bf )
                return;

            sensor_sample **tmp = samples;
            samples = ( sensor_sample** )malloc(sizeof(void*) * ( nSample + 1 ) );
            for ( int i = 0; i < nSample; i++ )
                samples[i] = tmp[i];
            samples[nSample++] = np;
            free( tmp );

        }//endif
    }

    sensor_sample **samples;
    int nSample;
}CXSampleTask;

typedef struct  CXUSampleTask {
    CXUSampleTask() {
        usamples = NULL;
        nUSample = 0;
    }

    void clear() {
        //all sample managed by sample_map gSampleMap
//        for ( int i = 0; i < nSample; i++ ) {
//            samples[i]->clear();
//            free( samples[i] );
//        }
        CXDFFREE( usamples );
        nUSample = 0;
    }

    void insert( sensor_usample *np ) {
        if ( !usamples ) {
            usamples = ( sensor_usample** )malloc(sizeof(void*) );
            usamples[0] = np;
            nUSample = 1;
        } else {
            bool bf = false;
            for ( int i = 0; i < nUSample; i++ ) {
                if ( usamples[i] == np ) {
                    bf = true;
                    break;
                }
            }
            if ( bf )
                return;

            sensor_usample **tmp = usamples;
            usamples = ( sensor_usample** )malloc(sizeof(void*) * ( nUSample + 1 ) );
            for ( int i = 0; i < nUSample; i++ )
                usamples[i] = tmp[i];
            usamples[nUSample++] = np;
            free( tmp );

        }//endif
    }

    sensor_usample **usamples;
    int nUSample;
}CXUSampleTask;

//////////////////////////////////// map ////////////////////////////////////
class sample_pair {
public:
    sample_pair() {
        sample = NULL;
//        usample = NULL;
        used = -1;
    }
    ~sample_pair() {
        clear();
    }

    void clear() {
        if ( sample && !used ) {
            sample->clear();
            free( sample );
        } else if ( usample && used == 1 ) {
            usample->clear();
            free( usample );
        }
        used = -1;
        key.clear();
    }

    sensor_sample* getSample() {
        if ( !used && sample )
            return sample;
        else
            return NULL;
    }
    sensor_usample* getUionSample() {
        if ( used == 1 && usample )
            return usample;
        else
            return NULL;
    }

    void setSample( sensor_sample *sp ) {
        used = 0;
        sample = sp;
    }
    void setUnionSample( sensor_usample *usp ) {
        used = 0;
        usample = usp;
    }

    int getKey() {
        return key.getKey();
    }
    char* getKeyStr() {
        return key.getKeyStr();
    }

    void setKey( int ikey ) {
        key.setKey( ikey );
    }
    void setKeyStr( const char *str ) {
        key.setKeyStr( str );
    }

    bool isEmpty() {
        if ( used == -1 && !sample )
            return true;
        return false;
    }

    int type() { return used; }
protected:
    union {
        sensor_sample *sample;
        sensor_usample *usample;
    };

    int used; //sample? or union sample = 0, 1
    sample_key key;
 };

typedef struct  sample_map {
    sample_map() {
        num = 0;
        last = 0;
    }
    void clear() {
        sample_pair *pair = map;
        sample_pair *limit = pair + last;
        while ( pair < limit ) {
            pair->clear();
            pair++;
        }
        num = 0;
        last = 0;
    }
    sample_pair map[CXDFSENSOR_MAX_NUM_SAMPLES];
    int num;
    int last;
}sample_map;

//////////////////////////////////// buffer ////////////////////////////////////
#define CXDF_SAMPLE_BUFFER_SIZE 1000
//for one sensor's data
typedef struct sensor_buffer {
    sensor_buffer() {
        tdatas = NULL;
        vals = NULL;
        last = NULL;
        nDataLimit = 0;
        nValLimit = 0;
        nData = 0;
        nVal = 0;
    }

    void clear() {
        CXDFFREE( tdatas );
        CXDFFREE( vals );
        nData = 0;
        nVal = 0;
    }

    sensor_TData *tdatas;
    double *vals;
    double *last;
    int nValLimit;
    int nDataLimit;
    int nData;
    int nVal;
}sensor_buffer;
//for one sample's sensors' data
typedef struct sample_buffer {
    sample_buffer() {
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
            sbuffer[i].tdatas = (sensor_TData*)malloc( sizeof(sensor_TData) * CXDF_SAMPLE_BUFFER_SIZE );
            sbuffer[i].vals = (double*)malloc( sizeof(double) * CXDF_SAMPLE_BUFFER_SIZE );
            sbuffer[i].last = sbuffer[i].vals;
            sbuffer[i].nDataLimit = CXDF_SAMPLE_BUFFER_SIZE;
            sbuffer[i].nValLimit = CXDF_SAMPLE_BUFFER_SIZE;
        }
    }

    void clear() {
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
            sbuffer[i].clear();
        }
    }

    void reset() {
        for ( int i = 0; i < CXDFSENSOR_NUM; i++ ) {
            sbuffer[i].nData = 0;
            sbuffer[i].nVal = 0;
            sbuffer[i].last = sbuffer[i].vals;
        }
    }

    sensor_buffer sbuffer[CXDFSENSOR_NUM];
}sample_buffer;

//////////////////////////////////// sample class ////////////////////////////////////

class CXSensorSample {
public:
    CXSensorSample();
    ~CXSensorSample();
    void clear();
    //read sensor file
    int readSensor( const char *path = sample_pathInfo );
    int readFileList( const char *filePath, int mask );
    int readSampleFromFile( FILE *fp, int* type, int nType, sensor_sample *sample );
    //write sensor sample struct
    void beginWriteTimeTag( int nid, int64_t tag );
    void writeVal( int nid, double *vals, int nval );
    void endWrite( int nid, int nval );
    void writeToSensorSample( sensor_sample *sample );
    void checkTouchSensor( sensor_data *touch );
    //derive union sample struct
    void getUnionSample();
    sensor_usample* getUnionSample( sensor_sample *sample );
    void checkSample( sensor_sample *sample );
    void sortTask();

    static SampleSize* globalSampleSize() { return &gSampleSize; }
    static sample_pair* findPair( int key );
    static sensor_sample* findSample( const char *keyStr );
    static sensor_usample* findUSample( const char *keyStr, int key );
    static sensor_sample* getSample( const char *keyStr );
    static sensor_usample* getUSample( const char *keyStr = NULL, int key = -1 );
    static int addSample( sensor_sample *sample );
    static int addUSample( sensor_usample *usample );
    static int printSampleToFile( sensor_sample *sample, const char *path );
    static void clearGlobalMap() {
        gSampleMap.clear();
        gSampleBuffer.clear();
    }

public:
    CXSampleTask m_task;
    CXUSampleTask m_utask;
    char *m_path;

    static sample_map gSampleMap;
    static sample_buffer gSampleBuffer;
    static SampleSize gSampleSize;
};

int getSensorType( char *chr );
int getSensorType( int bit );
unsigned int getSampleKeyID();
void getGlobalSampleSize( SampleSize *gSize, sensor_sample *sample );

void interSensorPoint( sensor_TData *ptr, int64_t t, float *val );
void accsSubGrav( sensor_sample *sample );


#endif // CXSENSORDATA_H
