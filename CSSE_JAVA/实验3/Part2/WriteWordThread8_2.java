package Part2;

class WriteWordThread8_2 extends Thread
{
    int n = 0;
    //设置进程名称，初始化n
    WriteWordThread8_2(String s, int n)
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