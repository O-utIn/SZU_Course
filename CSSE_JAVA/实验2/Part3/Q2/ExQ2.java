package Part3.Q2;

public class ExQ2 {
    public static void main(String[] args) {
        Vector vector=new Vector();
        int[] x={3,9,2,7};
        int[] y={2,-8,-1,6};

        //向量X
        System.out.print("向量x为：[");
        for(int i=0;i<x.length;i++){
            System.out.print(x[i]);
            if(i!=x.length-1){
                System.out.print(",");
            }
        }
        System.out.println("]");

        //向量Y
        System.out.print("向量y为：[");
        for(int i=0;i<y.length;i++){
            System.out.print(y[i]);
            if(i!=y.length-1){
                System.out.print(",");
            }
        }
        System.out.println("]");

        //相加
        int[] z=vector.add(x,y);
        System.out.print("它们的和为：[");
        for(int i=0;i<z.length;i++){
            System.out.print(z[i]);
            if(i!=z.length-1){
                System.out.print(",");
            }
        }
        System.out.println("]");

        //相减
        z=vector.minus(x,y);
        System.out.print("它们的差为：[");
        for(int i=0;i<z.length;i++){
            System.out.print(z[i]);
            if(i!=z.length-1){
                System.out.print(",");
            }
        }
        System.out.println("]");

        //点乘
        z=vector.elementwiseProduct(x,y);
        System.out.print("它们的点乘为：[");
        for(int i=0;i<z.length;i++){
            System.out.print(z[i]);
            if(i!=z.length-1){
                System.out.print(",");
            }
        }
        System.out.println("]");

        //内积
        int inner =vector.innerProduct(x,y);
        System.out.println("它们的内积为："+inner);

        //X的模
        double norm1=vector.norm(x);
        System.out.printf("向量x的模为：%.2f\n",norm1);

        //Y的模
        double norm2=vector.norm(y);
        System.out.printf("向量y的模为：%.2f\n",norm2);

        //根据模比较X、Y的大小
        System.out.println("向量的比较结果为："+vector.compare(x,y));
    }
}
