#include "jni_Demo.h"
#include <cstdlib>
#include <cstring>
#include <new>

static inline void swap(jint& a, jint& b) {
    jint t = a;
    a = b;
    b = t;
}

static void quickSortRange(jint* a, int left, int right) {
    if (left >= right) return;
    int i = left, j = right;
    jint pivot = a[(left + right) / 2];
    while (i <= j) {
        while (a[i] < pivot) ++i;
        while (a[j] > pivot) --j;
        if (i <= j) {
            swap(a[i], a[j]);
            ++i;
            --j;
        }
    }
    quickSortRange(a, left, j);
    quickSortRange(a, i, right);
}

static void quicksort(jint* a, int n) {
    quickSortRange(a, 0, n - 1);
}

JNIEXPORT jintArray JNICALL Java_jni_Demo_quickSort
  (JNIEnv *env, jobject /* obj */, jintArray arr)
{
    jsize len = env->GetArrayLength(arr);

    // Get elements from Java array into native buffer
    jint *buf = env->GetIntArrayElements(arr, nullptr);
    if (buf == nullptr) return nullptr;  // OutOfMemoryError already thrown

    // Allocate working copy
    jint* copy = new (std::nothrow) jint[len];
    if (copy == nullptr) {
        env->ReleaseIntArrayElements(arr, buf, JNI_ABORT);
        return nullptr;
    }
    for (jsize i = 0; i < len; ++i) copy[i] = buf[i];

    env->ReleaseIntArrayElements(arr, buf, JNI_ABORT);

    // Sort
    quicksort(copy, len);

    // Create result Java array
    jintArray result = env->NewIntArray(len);
    env->SetIntArrayRegion(result, 0, len, copy);

    delete[] copy;
    return result;
}

JNIEXPORT jlong JNICALL Java_jni_Demo_fibonacci
  (JNIEnv *env, jobject /* obj */, jint n)
{
    if (n < 0) {
        env->ThrowNew(env->FindClass("java/lang/IllegalArgumentException"),
                      "n must be >= 0");
        return 0;
    }
    if (n == 0) return 0;
    if (n == 1) return 1;

    jlong a = 0, b = 1;
    for (jint i = 2; i <= n; ++i) {
        jlong c = a + b;
        a = b;
        b = c;
    }
    return b;
}
