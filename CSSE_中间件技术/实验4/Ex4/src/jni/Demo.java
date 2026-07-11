package jni;


public class Demo {

    // Load the native library when this class is loaded
    static {
        System.loadLibrary("JniDemo");
    }

    public native int[] quickSort(int[] arr);
    public native long fibonacci(int n);
}
