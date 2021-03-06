/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class su_jake_hackrf_Hackrf */

#ifndef _Included_su_jake_hackrf_Hackrf
#define _Included_su_jake_hackrf_Hackrf
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     su_jake_hackrf_Hackrf
 * Method:    open
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_open
  (JNIEnv *, jclass);

/*
 * Class:     su_jake_hackrf_Hackrf
 * Method:    close
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_close
  (JNIEnv *, jclass);

/*
 * Class:     su_jake_hackrf_Hackrf
 * Method:    setFrequency
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_setFrequency
  (JNIEnv *, jclass, jint);

/*
 * Class:     su_jake_hackrf_Hackrf
 * Method:    setSampleRate
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_setSampleRate
  (JNIEnv *, jclass, jint);

/*
 * Class:     su_jake_hackrf_Hackrf
 * Method:    startRx
 * Signature: (Lsu/jake/hackrf/RxCallback;)I
 */
JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_startRx
  (JNIEnv *, jclass, jobject);

/*
 * Class:     su_jake_hackrf_Hackrf
 * Method:    stopRx
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_stopRx
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
