package Example8_2;

public class Example8_2
{
    public static void main(String args[])
    {
        WriteWordThread zhang, wang;
        //新建进程
        zhang = new WriteWordThread("Zhang", 200);
        wang = new WriteWordThread("Wang", 100);
        //进程进入排队
        zhang.start();
        wang.start();
    }
}

class WriteWordThread extends Thread
{
    int n = 0;
    //设置进程名称，初始化n
    WriteWordThread(String s, int n)
    {
        setName(s);
        this.n = n;
    }
    //运行进程
    public void run()
    {
        for(int i=1; i<=3; i++)
        {
            System.out.println("Thread: " + getName());
            //等待n毫秒
            try
            {
                sleep(n);
            }
            catch(InterruptedException e) {}
        }
    }
}