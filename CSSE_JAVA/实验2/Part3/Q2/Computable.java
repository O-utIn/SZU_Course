package Part3.Q2;

public interface Computable {
    int[] add(int[] a, int[] b);
    int[] minus(int[] a,int[] b);
    int[] elementwiseProduct(int[] a,int[] b);
    int innerProduct(int[] a,int[] b);
    double norm(int[] x);
    String compare(int[] a,int[] b);
}
