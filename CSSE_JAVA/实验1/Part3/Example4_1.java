package Part3;

class Circle //Circle类
{
    double radius; //半径
    double getArea() //定义方法，用于计算面积
    {
        double area = 3.14*radius*radius; //3.14*r*r
        return area;
    }
}

public class Example4_1 { //主类
    public static void main(String[] args) //程序入口
    {
        Circle circle;
        circle = new Circle(); //创建一个Circle类的对象
        circle.radius = 1; //设置半径为1
        double area = circle.getArea(); //计算这个圆的面积
        System.out.println(area); //打印面积
    }
}
