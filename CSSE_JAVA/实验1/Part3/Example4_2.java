package Part3;

class Ladder //定义一个梯形类
{
    //成员数据（上底、下底、高）
    double above,bottom,height;
    //默认构造方法
    Ladder(){}
    //自定义构造方法
    Ladder(double a,double b,double h)
    {
        above=a;
        bottom=b;
        height=h;
    }
    //定义一个方法用于设置梯形的上底
    public void setAbove(double a)
    {
        above=a;
    }
    //定义一个方法用于设置设置梯形的下底
    public void setBottom(double b)
    {
        bottom=b;
    }
    //定义一个方法用于设置梯形的高
    public void setHeight(double h)
    {
        height=h;
    }
    //用于计算并返回该梯形的面积
    double computeArea()
    {
        return (above+bottom)*height/2.0;
    }
}

public class Example4_2 {
    public static void main(String args[]) //主程序入口
    {
        double area1=0,area2=0; //设置两个变量用于存储两个梯形的面积
        Ladder ladderOne,ladderTwo; //创建两个梯形
        ladderOne=new Ladder(); //用不含参的构造方法创建第一个梯形
        ladderTwo=new Ladder(10,88,20); //用含参的构造方法创建第二个梯形
        ladderOne.setAbove(16); //设置第一个梯形的上底
        ladderOne.setBottom(26); //设置第一个梯形的下底
        ladderOne.setHeight(100); //设置第一个梯形的高
        ladderTwo.setAbove(300); //更新第二个梯形的上底
        ladderTwo.setBottom(500); //更新第二个梯形的下底
        area1=ladderOne.computeArea(); //调用计算面积的方法，area1用于存储第一个梯形的面积
        area2=ladderTwo.computeArea(); //area2用于存储第二个梯形的面积
        //打印两个梯形的面积
        System.out.println(area1);
        System.out.println(area2);
    }
}
