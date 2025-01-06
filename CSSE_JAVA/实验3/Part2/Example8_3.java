package Part2;

public class Example8_3
{
    public static void main(String args[])
    {
        //新建left和right进程
        Left left = new Left();
        Right right = new Right();
        //进入排队
        left.start();
        right.start();
        while(true)
        {
            //等待100ms
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            //某一方大于等于8时输出并结束
            if(left.n>=8 || right.n>=8)
            {
                System.out.println(left.n + "," + right.n);
                System.exit(0);
            }
        }
    }
}