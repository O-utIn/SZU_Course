package Part3;

class LadderA  //定义一个梯形类
{
    //成员数据（上底、下底、高）
    double above,height; //实例变量
    static double bottom; //静态变量
    //定义一个方法用于设置梯形的上底
    void setAbove(double a)
    {
        above=a;
    }
    //定义一个方法用于设置设置梯形的下底
    void setBottom(double b)
    {
        bottom=b;
    }
    //定义一个方法用于获得梯形的上底
    double getAbove()
    {
        return above;
    }
    //定义一个方法用于获得梯形的下底
    double getBottom()
    {
        return bottom;
    }
}

public class Example4_3 {
    public static void main(String args[]) //主程序入口
    {
        LadderA.bottom=60;//设置梯形的下底默认为60
        LadderA ladderOne,ladderTwo;  //创建两个梯形
        System.out.println(LadderA.bottom); //打印梯形的下底
        //创建两个梯形
        ladderOne=new LadderA();
        ladderTwo=new LadderA();
        System.out.println(ladderOne.getBottom());  //打印第一个梯形的下底
        System.out.println(ladderTwo.getBottom());  //打印第二个梯形的下底
        ladderOne.setAbove(11);  //设置第一个梯形的上底
        ladderTwo.setAbove(22);  //设置第二个梯形的上底
        ladderTwo.setBottom(100);  //更新第二个梯形的下底
        System.out.println(LadderA.bottom); //打印梯形的默认下底
        System.out.println(ladderOne.getAbove());  //打印第一个梯形的上底
        System.out.println(ladderTwo.getAbove());  //打印第二个梯形的上底
    }
}
