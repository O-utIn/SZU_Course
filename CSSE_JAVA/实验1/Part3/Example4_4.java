package Part3;

class Tom  //定义一个Tom类
{
    final int MAX=100;  //定义MAX为100
    final static int MIN=20;  //定义MIN为20
}

public class Example4_4 {
    public static void main(String args[])  //主程序入口
    {
        System.out.println(Tom.MIN); //打印Tom类的MIN
        //System.out.println(Tom.MAX); // Error
        Tom cat = new Tom();  //创建一个Tom类对象cat
        System.out.println(cat.MAX); //打印cat的MAX
    }
}
