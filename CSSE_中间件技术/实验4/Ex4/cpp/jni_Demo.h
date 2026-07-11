#include <jni.h>

#ifndef _Included_jni_Demo
#define _Included_jni_Demo
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jintArray JNICALL Java_jni_Demo_quickSort
  (JNIEnv *, jobject, jintArray);


JNIEXPORT jlong JNICALL Java_jni_Demo_fibonacci
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
