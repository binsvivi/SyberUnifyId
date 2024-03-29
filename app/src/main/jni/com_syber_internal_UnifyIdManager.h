/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_syber_internal_UnifyIdManager */

#ifndef _Included_com_syber_internal_UnifyIdManager
#define _Included_com_syber_internal_UnifyIdManager
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_setup
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1setup
  (JNIEnv *, jclass);

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_featSlides
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1featSlides
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_featGesture
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1featGesture
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_TrainGesture
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1TrainGesture
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_TrainSVM
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_syber_internal_UnifyIdManager_native_1TrainSVM
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_testSlide
 * Signature: (Ljava/lang/String;Ljava/lang/String;)D
 */
JNIEXPORT jdouble JNICALL Java_com_syber_internal_UnifyIdManager_native_1testSlide
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_syber_internal_UnifyIdManager
 * Method:    native_testGesture
 * Signature: (Ljava/lang/String;Ljava/lang/String;)D
 */
JNIEXPORT jdouble JNICALL Java_com_syber_internal_UnifyIdManager_native_1testGesture
  (JNIEnv *, jclass, jstring, jstring);

#ifdef __cplusplus
}
#endif
#endif
