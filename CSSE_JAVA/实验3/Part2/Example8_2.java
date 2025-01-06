package Part2;

public class Example8_2
{
    public static void main(String args[])
    {
        WriteWordThread8_2 zhang, wang;
        //新建进程
        zhang = new WriteWordThread8_2("Zhang", 200);
        wang = new WriteWordThread8_2("Wang", 100);
        //进程进入排队
        zhang.start();
        wang.start();
    }
}