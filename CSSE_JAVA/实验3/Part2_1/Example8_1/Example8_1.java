package Example8_1;

public class Example8_1
{
	public static void main(String args[])
	{
		WriteWordThread zhang, wang;
		zhang = new WriteWordThread("Zhang"); //新建线程
		wang = new WriteWordThread("Wang"); //新建线程
		zhang.start(); //启动线程
		for(int i=1; i<=3; i++)
		{
			System.out.println("Main Thread");
		}
		wang.start(); //启动线程
	}
}

class WriteWordThread extends Thread
{
    //传入进程名称
    WriteWordThread(String s)
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
