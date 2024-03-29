/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <UnifyIdManager.h>
/* Header for class com_syber_internal_UnifyIdManager */

#ifndef _Included_com_syber_internal_UnifyIdManager
#define _Included_com_syber_internal_UnifyIdManager
#ifdef __cplusplus
extern "C" {
#endif

namespace android {
    static UnifyIdManager *unifyid_manager;

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_setup
 * Signature: ()V
 */
    JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1setup
            (JNIEnv *, jclass) {
        unifyid_manager = new UnifyIdManager();
    }

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_featSlides
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
    JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1featSlides
            (JNIEnv * env, jclass, jstring samplePath, jstring featPath) {
        const char *c_sample_path = env->GetStringUTFChars(samplePath, NULL);
        const char *c_feat_path = env->GetStringUTFChars(featPath, NULL);

        std::string fsample(c_sample_path);
        std::string ffeat(c_feat_path);


        unifyid_manager->featSlide(fsample.c_str(), ffeat.c_str());
    }

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_featGesture
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
    JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1featGesture
            (JNIEnv * env, jclass, jstring samplePath, jstring featPath) {
        const char *c_sample_path = env->GetStringUTFChars(samplePath, NULL);
        const char *c_feat_path = env->GetStringUTFChars(featPath, NULL);

        std::string fsample(c_sample_path);
        std::string ffeat(c_feat_path);


        unifyid_manager->featGesture(fsample.c_str(), ffeat.c_str());
    }

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_TrainGesture
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
    JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1TrainGesture
            (JNIEnv *env, jclass, jstring featPath, jstring savePath) {
        //ALOGV("TrainGesture");
        const char *c_feat_path = env->GetStringUTFChars(featPath, NULL);
        const char *c_save_path = env->GetStringUTFChars(savePath, NULL);

        std::string ffeat(c_feat_path);
        std::string fsave(c_save_path);


        unifyid_manager->trainGesture(ffeat.c_str(), fsave.c_str());
    }

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_TrainSVM
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
    JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1TrainSVM
            (JNIEnv *env, jclass, jstring featPath, jstring savePath) {
        const char *c_feat_path = env->GetStringUTFChars(featPath, NULL);
        const char *c_save_path = env->GetStringUTFChars(savePath, NULL);

        std::string ffeat(c_feat_path);
        std::string fsave(c_save_path);


        //ALOGV("requestTrainSVM");
        unifyid_manager->trainSVM(ffeat.c_str(), fsave.c_str());
    }

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_testSlide
 * Signature: (Ljava/lang/String;Ljava/lang/String;)D
 */
    JNIEXPORT jdouble JNICALL Java_com_syber_internal_UnifyIdManager_native_1testSlide
            (JNIEnv *env, jclass, jstring modulePath, jstring featPath) {
        const char *c_module_path = env->GetStringUTFChars(modulePath, NULL);
        const char *c_feat_path = env->GetStringUTFChars(featPath, NULL);

        std::string fmodule(c_module_path);
        std::string ffeat(c_feat_path);

        double score = unifyid_manager->testSlide(fmodule.c_str(), ffeat.c_str());
        return score;
    }

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_testGesture
 * Signature: (Ljava/lang/String;Ljava/lang/String;)D
 */
    JNIEXPORT jdouble JNICALL Java_com_syber_internal_UnifyIdManager_native_1testGesture
            (JNIEnv *env, jclass, jstring modulePath, jstring featPath) {
        const char *c_module_path = env->GetStringUTFChars(modulePath, NULL);
        const char *c_feat_path = env->GetStringUTFChars(featPath, NULL);

        std::string fmodule(c_module_path);
        std::string ffeat(c_feat_path);

        double score = unifyid_manager->testGesture(fmodule.c_str(), ffeat.c_str());
        return score;
    }
}
#ifdef __cplusplus
}
#endif
#endif
