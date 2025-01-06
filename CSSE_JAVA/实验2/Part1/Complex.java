package Part1;

public class Complex {
    private double realPart; //实数部分
    private double imagePart; //虚数部分

    //不含参数的构造方法
    public Complex() {
        this.realPart = 0;
        this.imagePart = 0;
    }

    //带参数的构造方法
    public Complex(double r, double i) {
        this.realPart = r;
        this.imagePart = i;
    }

    //当前复数与另一个相减
    public Complex ComplexcomplexSub(Complex c) {
        return new Complex(this.realPart - c.realPart, this.imagePart - c.imagePart);
    }

    //当前复数与另一个相乘
    public Complex ComplexcomplexMult(Complex c) {
        return new Complex(this.realPart * c.realPart - this.imagePart * c.imagePart,
                this.imagePart * c.realPart + this.realPart * c.imagePart);
    }

    public String toString() {
        if(this.imagePart > 0)
            return this.realPart + " +" + this.imagePart + "i";
        else if(this.imagePart < 0)
            return this.realPart +" "+ this.imagePart + "i";
        else if(this.realPart == 0)
            return this.imagePart + "i";
        else
            return this.realPart + ",The image is zero";
    }

    public static void main(String[] args) {
        //以3+5i和2+7i为例
        Complex c1 = new Complex(3, 5);
        Complex c2 = new Complex(2, 7);
        System.out.println(c1.toString());
        System.out.println(c2.toString());
        System.out.println("c1-c2="+c1.ComplexcomplexSub(c2));
        System.out.println("c1*c2="+c1.ComplexcomplexMult(c2));
    }

}
