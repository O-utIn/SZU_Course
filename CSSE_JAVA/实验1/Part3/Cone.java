package Part3;

public class Cone {
    private double radius;    // 圆锥体的底面半径
    private double height;    // 圆锥体的高

    // 创建指定半径和高度的圆锥体
    public Cone(double radius, double height)
    {
        this.radius=radius;
        this.height=height;
    }

    // 设置圆锥体的底面半径和高
    public void setInfo(double radius,double height)
    {
        this.radius=radius;
        this.height = height;
    }

    // 计算圆锥体的体积
    public double volume()
    {
        //(1/3) * π * r^2 * h
        return (1.0/3) * Math.PI * Math.pow(radius, 2) * height;
    }

    // 计算圆锥体的表面积
    public double area()
    {
        //π * r^2 + π * r * l，其中l是斜高，使用勾股定理计算l
        double l = Math.sqrt(Math.pow(radius, 2) + Math.pow(height, 2));
        return Math.PI * Math.pow(radius, 2) + Math.PI * radius * l;
    }

    // 将圆锥体的属性转换为字符串表示
    public String toString()
    {
        return "Cone{" +
                "radius=" + radius +
                ", height=" + height +
                ", volume=" + volume() +
                ", surfaceArea=" + area() +
                '}';
    }

    public static void main(String[] args) {
        Cone C = new Cone(5.0, 10.0); // 创建一个底面半径为5，高为10的圆锥体
        System.out.println(C.toString()); // 打印圆锥体的信息

        C.setInfo(2,5); //修改信息
        System.out.println("volume="+C.volume()); //体积
        System.out.println("area="+C.area()); //表面积
        System.out.println(C.toString()); // 打印更新后Cone对象的信息
    }
}
