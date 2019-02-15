#define LOG_NDEBUG 0
#define LOG_NDDEBUG 0

#include "UnifyIdManager.h"
//#include "NotifyFunctor.h"
//#include <log/log.h>
#include <data/cxslide.h>
#include <data/cxgesture.h>
#include <data/cxsensordata.h>
#include <module/cxslidemodule.h>
#include <module/cxgesturemodule.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
//#include <log/log.h>

#include <android/log.h>
//#define LOG_TAG "System.out.c"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#define FIND_CMD "find "
#define RM_CMD "rm "
#define MKDIR_CMD "mkdir -p "
#define TOUCH_CMD "touch "
#define LOG_TAG "unifyIdManager "


namespace android {
/*
    static void printKeyedVector(KeyedVector<String8, String8> vector) {
        ALOGD("print cache");
        for (size_t i = 0; i < vector.size(); ++i) {
            ALOGD("key %s, value %s", vector.keyAt(i).string(), vector.valueAt(i).string());
        }
        ALOGD("print cache end");
    }
*/
    static const char reservePrefix[] = "com.syber";
    static const char regex[] = "^([_[:alnum:]]+\\.)*[_[:alnum:]]+$";

    UnifyIdManager::UnifyIdManager()
  //  :mNotifyFunctor(NULL)
    {
    //    ALOGI("UnifyId Manager is creating...");
   //     if (0 != regcomp(&re, regex, REG_EXTENDED | REG_NOSUB)) {

    //    }
	std::string s = "";
	std::string tmpcmd, cmdStr, rescmd,rescmd2,rescmd3, rescmd4, cmdStr2, cmdStrInfo, cmdStrInfo2, cmdStrInfo3,cmdStrInfo4,cmdStrInfo5, cmdStrInfo6,cmdStr3,cmdStr4, cmdStr5, cmdStr6,cmdStr7, cmdStr8, cmdStr9, cmdStr10, cmdStr11, cmdStr12;
	tmpcmd = "/sdcard";
	rescmd = tmpcmd + "/SensorUnifyIdData";
	rescmd2 = tmpcmd + "/UnifyID";
	rescmd3 = tmpcmd + "/UnifyID/feature";
	rescmd4 = tmpcmd + "/UnifyID/module";

	cmdStr = std::string(MKDIR_CMD) + " " + rescmd + "/gesture";
	cmdStrInfo5 = std::string(TOUCH_CMD) + " " + rescmd + "/gesture/gesture-sample.info";
	cmdStrInfo6 = std::string(TOUCH_CMD) + " " + rescmd + "/gesture/gesture-test.info";
	cmdStr4 = std::string(MKDIR_CMD) + " " + rescmd3+"/gesture/test";
	cmdStr10 = std::string(MKDIR_CMD) + " " + rescmd3 +"/gesture/train";
	cmdStr7 = std::string(MKDIR_CMD) + " " + rescmd4 + "/gesture";
	system(cmdStr.c_str());
	system(cmdStr4.c_str());
	system(cmdStr10.c_str());
	system(cmdStr7.c_str());
	cmdStr2 = std::string(MKDIR_CMD) +" " + rescmd + "/slide";
	cmdStrInfo = std::string(TOUCH_CMD) +" " + rescmd + "/slide/slide-sample.info";
	cmdStrInfo2 = std::string(TOUCH_CMD) +" " + rescmd +"/slide/slide-test.info";
	cmdStr5 = std::string(MKDIR_CMD) +" " + rescmd3 + "/slide/test";
	cmdStr11 = std::string(MKDIR_CMD) +" " + rescmd3 +"/slide/train";
	cmdStr8 = std::string(MKDIR_CMD) +" " + rescmd4 + "/slide";
	system(cmdStr2.c_str());
	system(cmdStr5.c_str());
	system(cmdStr11.c_str());
	system(cmdStr8.c_str());
	cmdStr3 = std::string(MKDIR_CMD) + " " + rescmd+ "/unlock";
	cmdStrInfo3 = std::string(TOUCH_CMD) +" " + rescmd + "/unlock/unlock-sample.info";
	cmdStrInfo4 = std::string(TOUCH_CMD) +" " + rescmd +"/unlock/unlock-test.info";
	cmdStr6 = std::string(MKDIR_CMD) +" " + rescmd3 + "/unlock/test";
	cmdStr12 = std::string(MKDIR_CMD) +" " + rescmd3+"/unlock/train";
	cmdStr9 = std::string(MKDIR_CMD) +" " + rescmd4 +"/unlock";
	system(cmdStr3.c_str());
	system(cmdStr6.c_str());
	system(cmdStr12.c_str());
	system(cmdStrInfo5.c_str());
	system(cmdStrInfo6.c_str());
	system(cmdStrInfo.c_str());
	system(cmdStrInfo2.c_str());
	system(cmdStrInfo3.c_str());
	system(cmdStrInfo4.c_str());
        //testSlide(s);
 //       ssalWrapper = new ssalConfigWrapper();
 //       mNotifyFunctor = new NotifyFunctor(*this);
 //       ssalWrapper->registerCallback(mNotifyFunctor);
 //       initCache();
    }

    UnifyIdManager::~UnifyIdManager() {
    //    ssalWrapper->unregisterCallback();
    //    delete mNotifyFunctor;
    //    regfree(&re);
    }

    void UnifyIdManager::featGesture(std::string samplePath, std::string featPath)
    {
	 CXSensorSample sampler;

   	 const char* sample_path = "/sdcard/samples.info";
   	// const char *featPath = "/sdcard/UnifyID/feature/slide/train/";
	std::string samplePath8 = samplePath;
	std::string featPath8 = featPath;
	std::string tmpcmd, cmdStr, rescmd, cmdStr2;
	tmpcmd = featPath8;
        cmdStr2 = std::string(RM_CMD) + " " + tmpcmd +"/*.feat";
        //ALOGI("UnifyId begin featGesture...%s   %s",samplePath8.string(), featPath8.string());
   	 featGestures( &sampler, samplePath8.c_str(), featPath8.c_str());
       // ALOGI("UnifyId end featGesture...");
   	 sampler.clear();
      //  char cmd[MAX_NAME_LEN];
       // memset(cmd,0,MAX_NAME_LEN);
//	String8 tmpcmd, cmdStr, rescmd, cmdStr2;
//	tmpcmd = tmpcmd.format(featPath8.string());
//	rescmd = rescmd.format("%s/result.features", featPath8.string());
//	cmdStr = cmdStr.format("%s %s -name \"*.feat\" >%s", FIND_CMD, tmpcmd.string(), rescmd.string());
//	system(cmdStr.string());
//        cmdStr2 = cmdStr2.format("%s %s/*.feat", RM_CMD, tmpcmd.string());
//	system(cmdStr2.string());
    }

    void UnifyIdManager::trainGesture(std::string featPath, std::string savePath)
    {
//	 const char *path = "/sdcard/UnifyID/slideModule";
//	 const char *feat_Path = "/sdcard/UnifyID/feature/slide/train/xizhi.feaures";

	 CxGestureModule module;
	 std::string featPath8 = featPath;
	 std::string savePath8 = savePath;
	 //ALOGI("UnifyIdManager::trainGesture %s, save: %s", featPath8.string(), savePath8.string());
	std::string tmpcmd, cmdStr, rescmd, cmdStr2;
	tmpcmd = featPath8;
	rescmd = featPath8 + "/result.features";
	cmdStr = std::string(FIND_CMD) +" " + tmpcmd +" " + "-name \"*.feat\" >" + rescmd;
	system(cmdStr.c_str());
	 module.loadFeats( rescmd.c_str());
	 module.factored( GESTURE_FEATURE_DIM_SIZE );
	 module.dynaCluster( 1, 0 );
	// long long st = systemTime(SYSTEM_TIME_MONOTONIC);
	 module.train();
	// long long et = systemTime(SYSTEM_TIME_MONOTONIC);
    //	 ALOGD("trainSVM: time diff:%lld",et-st);
	 module.save(savePath8.c_str());
	 module.clear();

    }

    double UnifyIdManager::testGesture(std::string modulePath, std::string featPath)
    {
	 const char *path = "/sdcard/UnifyID/module/slide/xizhi/slideModule";
   	 const char *feat_Path = "/sdcard/UnifyID/feature/slide/test/shabin-new.features";
	 std::string modulePath8 = modulePath;
	 std::string featPath8 = featPath;

	 //ALOGI("UnifyIdManager::testGesture %s, save: %s", featPath8.string(), modulePath8.string());
	std::string tmpcmd, cmdStr, rescmd, cmdStr2;
	tmpcmd = featPath8;
	rescmd = featPath8 + "/test.features";
	cmdStr = std::string(FIND_CMD) + " " + tmpcmd + "-name \"*.feat\" >"+ rescmd;
	system(cmdStr.c_str());
   	 CxGestureModule module;
	 module.load(modulePath8.c_str());
         
	 CxGestureModule tx;   
   	 tx.loadFeats( rescmd.c_str() );

   	 double p = 0.;
   	 int n = tx.nodeSize();
   	 LOGD("%d ==== \n",n);
   	 char** fpath = (char**)malloc( sizeof(char*) * n );
   	 FILE *fp = fopen( rescmd.c_str(), "r" );
   	 if ( fp ) {
   	     char buffer[1024] = {0};
   	     int len = 0;
   	     int i = 0;
   	     while( fscanf(fp, "%s", buffer) != EOF ) {
   	         len = strlen(buffer);
   	         char *tmp = (char*)malloc(len+1);
   	         memcpy( tmp, buffer, len );
   	         tmp[len] = 0;
   	         fpath[i++] = tmp;
   	         memset( buffer, 0, 1024 );
   	     }
   	 }

	 double total_scores = 0.f;
    	 int label = 0;
    	 int tnum = 0;
    	 int fnum = 0;
   	 for ( int i = 0; i < n; i++ ) {
	     //long long st = systemTime(SYSTEM_TIME_MONOTONIC);
   	     //p = svm_predict( module.module(), module.nodes()[i] );
	     p = module.test( tx.nodes()[i] ,label);
   	     //fprintf( stderr, "%s :  %lf \n", fpath[i], p );
	     //results.add(i, p);
	     if ( p < 0 )
		     fnum++;
	     else
		     tnum++;
	     fprintf( stderr, "%s :  [%d] model probability %lf \n", fpath[i], label, p );
	     total_scores += p;
           //  long long et = systemTime(SYSTEM_TIME_MONOTONIC);
           //  ALOGD("testGesture: time diff:%lld",et-st);
   	     //printf("%s :  %lf \n", fpath[i], p);
    	    // ALOGD("%s : %lf",fpath[i], p);
   	     free( fpath[i] );
   	 }

   	 free( fpath );
	    fprintf( stderr, "test %d ture feature, accept rate = %lf\n", n, (double)tnum / n );

        cmdStr2 = std::string(RM_CMD) +" " + tmpcmd + "/*.feat";
	system(cmdStr2.c_str());

	return total_scores;
    }

    void UnifyIdManager::featSlide(std::string samplePath, std::string featPath)
    {

	 CXSensorSample sampler;

   	 const char* sample_path = "/sdcard/samples.info";
   	// const char *featPath = "/sdcard/UnifyID/feature/slide/train/";
	std::string samplePath8 = samplePath;
	std::string featPath8 = featPath;
	std::string tmpcmd, cmdStr, rescmd, cmdStr2;
	tmpcmd = featPath8;
        cmdStr2 = std::string(RM_CMD) +" " + tmpcmd +"/*.feat";
	system(cmdStr2.c_str());
        //ALOGI("UnifyId begin featSlide...%s   %s",samplePath8.string(), featPath8.string());
   	 featSlides( &sampler, samplePath8.c_str(), featPath8.c_str() );
        //ALOGI("UnifyId end featSlide...%d", sampler.m_task.nSample);
   	 sampler.clear();
      //  char cmd[MAX_NAME_LEN];
       // memset(cmd,0,MAX_NAME_LEN);
//	String8 tmpcmd, cmdStr, rescmd, cmdStr2;
//	tmpcmd = tmpcmd.format(featPath8.string());
//	rescmd = rescmd.format("%s/result.features", featPath8.string());
//	cmdStr = cmdStr.format("%s %s -name \"*.feat\" >%s", FIND_CMD, tmpcmd.string(), rescmd.string());
//	system(cmdStr.string());
//        cmdStr2 = cmdStr2.format("%s %s/*.feat", RM_CMD, tmpcmd.string());
//	system(cmdStr2.string());
    }

    void UnifyIdManager::trainSVM(std::string featPath, std::string savePath)
    {
	 const char *path = "/sdcard/UnifyID/slideModule";
	 const char *feat_Path = "/sdcard/UnifyID/feature/slide/train/xizhi.feaures";
	 //ALOGI("UnifyIdManager::trainSVM %s, save: %s", featPath.string(), savePath.string());
	 CxSlideModule module;
	 std::string featPath8 = featPath;
	 std::string savePath8 = savePath;
	std::string tmpcmd, cmdStr, rescmd, cmdStr2;
	tmpcmd = featPath8;
	rescmd = featPath8 +"/result.features";
	cmdStr = std::string(FIND_CMD) + " "+  tmpcmd +" -name \"*.feat\" >"+ rescmd;
	system(cmdStr.c_str());
	 module.loadFeats( rescmd.c_str());
	 module.factored( SLIDE_FEATURE_DIM_SIZE );
	 module.dynaCluster( 1, 0 );
	 //long long st = systemTime(SYSTEM_TIME_MONOTONIC);
	 module.train();
	 //long long et = systemTime(SYSTEM_TIME_MONOTONIC);
    	// ALOGD("trainSVM: time diff:%lld",et-st);
	 module.save(savePath8.c_str());
	 module.clear();

    }

    double UnifyIdManager::testSlide(std::string modulePath, std::string featPath)
    {
	 const char *path = "/sdcard/UnifyID/module/slide/xizhi/slideModule";
   	 const char *feat_Path = "/sdcard/UnifyID/feature/slide/test/shabin-new.features";
	 std::string modulePath8 = modulePath;
	 std::string featPath8 = featPath;

	 //ALOGI("UnifyIdManager::testSLide %s, save: %s", featPath8.string(), modulePath8.string());
	std::string tmpcmd, cmdStr, rescmd, cmdStr2;
	tmpcmd = featPath8;
	rescmd = featPath8 + "/test.features";
	cmdStr = std::string(FIND_CMD) +" " + tmpcmd +" -name \"*.feat\" >"+rescmd;
	system(cmdStr.c_str());
   	 CxSlideModule module;
        LOGD("%s--testSlide---\n",modulePath8.c_str());
	 module.load(modulePath8.c_str());
	 CxSlideModule tx;
   	 tx.loadFeats( rescmd.c_str() );

   	 double p = 0.;
   	 int n = tx.nodeSize();
	 //ALOGI("FEATURE size: %d", n);
   	 //printf("%d ==== \n",n);
        LOGD("%d ==== \n",n);

        char** fpath = (char**)malloc( sizeof(char*) * n );
   	 FILE *fp = fopen( rescmd.c_str(), "r" );
   	 if ( fp ) {
   	     char buffer[1024] = {0};
   	     int len = 0;
   	     int i = 0;
   	     while( fscanf(fp, "%s", buffer) != EOF ) {
   	         len = strlen(buffer);
   	         char *tmp = (char*)malloc(len+1);
   	         memcpy( tmp, buffer, len );
   	         tmp[len] = 0;
   	         fpath[i++] = tmp;
   	         memset( buffer, 0, 1024 );
   	     }
	    fclose(fp);
   	 }

	 double total_scores = 0.f;
 	   int label = 0;
 	   fprintf( stderr, "//////////////////////// true sample //////////////////////// \n");
 	   int tnum = 0;
 	   int fnum = 0;

   	 for ( int i = 0; i < n; i++ ) {
	    // long long st = systemTime(SYSTEM_TIME_MONOTONIC);
   	   //  p = svm_predict( module.module(), module.nodes()[i] );
	     label = 0;
	     p = module.test( tx.nodes()[i], label );
   	    // fprintf( stderr, "%s :  %lf \n", fpath[i], p );
        	fprintf( stderr, "%s :  [%d] model probability %lf \n", fpath[i], label, p );
      //  	free( fpath[i] );
       		 if ( p < 0 )
       		     fnum++;
       		 else
       		     tnum++;
	     //results.add(i, p);
	     total_scores += p;
     //        long long et = systemTime(SYSTEM_TIME_MONOTONIC);
  //           ALOGD("testSlide: time diff:%lld",et-st);
   	     //printf("%s :  %lf \n", fpath[i], p);
//    	     ALOGD("%s : %lf",fpath[i], p);
   	     free( fpath[i] );
   	 }
    	free( fpath );
    	fprintf( stderr, "test %d ture feature, accept rate = %lf\n", n, (double)tnum / n );
   	     fprintf( stderr, "total score : %lf \n", total_scores );

   //	 free( fpath );

        cmdStr2 = std::string(RM_CMD) +" " + tmpcmd +"/*.feat";
	system(cmdStr2.c_str());
	return total_scores;
    }

};
