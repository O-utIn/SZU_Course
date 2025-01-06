package Part3.Q2;

public class Vector implements Computable {
    public int[] x=new int[4];

    //相加
    public int[] add(int[] a, int[] b){
        int[] c=new int[4];
        c[0]=a[0]+b[0];
        c[1]=a[1]+b[1];
        c[2]=a[2]+b[2];
        c[3]=a[3]+b[3];
        return c;
    }

    //相减
    public int[] minus(int[] a, int[] b){
        int[] c=new int[4];
        c[0]=a[0]-b[0];
        c[1]=a[1]-b[1];
        c[2]=a[2]-b[2];
        c[3]=a[3]-b[3];
        return c;
    }

    //点乘
    public int[] elementwiseProduct(int[] a, int[] b){
        int[] c=new int[4];
        c[0]=a[0]*b[0];
        c[1]=a[1]*b[1];
        c[2]=a[2]*b[2];
        c[3]=a[3]*b[3];
        return c;
    }

    //内积
    public int innerProduct(int[] a, int[] b){
        int c=0;
        for(int i=0;i<4;i++){
            c=c+a[i]*b[i];
        }
        return c;
    }

    //模
    public double norm(int[] x){
        int c=0;
        for(int i=0;i<4;i++){
            c=c+(x[i]*x[i]);
        }
        return Math.sqrt(c);
    }

    //根据模比较大小
    public String compare(int[] a, int[] b){
        int c1=0;
        int c2=0;
        for(int i=0;i<4;i++){
            c1=c1+a[i]*a[i];
            c2=c2+b[i]*b[i];
        }
        if(c1==c2)
            return "norm(a)=norm(b)";
        else if(c1<c2)
            return "norm(a)<norm(b)";
        else
            return "norm(a)>norm(b)";

    }

}
