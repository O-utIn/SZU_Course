package Part2;


public class Example8_1
{
	public static void main(String args[])
	{
		WriteWordThread8_1 zhang, wang;
		zhang = new WriteWordThread8_1("Zhang"); //新建线程
		wang = new WriteWordThread8_1("Wang"); //新建线程
		zhang.start(); //启动线程
		for(int i=1; i<=3; i++)
		{
			System.out.println("Main Thread");
		}
		wang.start(); //启动线程
	}
}
