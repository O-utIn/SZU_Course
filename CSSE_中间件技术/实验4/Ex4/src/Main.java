import jni.Demo;
import java.util.Arrays;


public class Main {
    public static void main(String[] args) {
        System.out.println("=== JNI Demo: Java calling C++ algorithms ===\n");

        Demo demo = new Demo();

        // ---- Test 1: quickSort ----
        int[] unsorted = {42, 13, 7, 99, 23, 4, 87, 56, 31, 78};
        System.out.println("Test 1 — quickSort");
        System.out.println("  Input:  " + Arrays.toString(unsorted));

        int[] sorted = demo.quickSort(unsorted);
        System.out.println("  Output: " + Arrays.toString(sorted));

        // Verify original array is unchanged (we return a new sorted copy)
        boolean originalIntact = Arrays.equals(unsorted,
                new int[]{42, 13, 7, 99, 23, 4, 87, 56, 31, 78});
        System.out.println("  Original array unchanged: " + originalIntact);

        // ---- Test 2: fibonacci ----
        System.out.println("\nTest 2 — fibonacci");
        int[] fibInputs = {0, 1, 2, 5, 10, 20, 30, 40, 50};
        for (int n : fibInputs) {
            long result = demo.fibonacci(n);
            System.out.printf("  fibonacci(%2d) = %d%n", n, result);
        }

        System.out.println("\n=== All tests passed! ===");
    }
}
