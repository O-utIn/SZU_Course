package Part2;

class WriteWordThread8_1 extends Thread
{
    //传入进程名称
    WriteWordThread8_1(String s)
    {
        setName(s);
    }
    //运行进程
    public void run()
    {
        for(int i=1; i<=3;i++)
            System.out.println("Thread: " + getName());
    }
}
