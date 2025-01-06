package Part2;

public class Example8_6
{
    public static void main(String args[])
    {
        String s1 = "ZHANG San";
        String s2 = "LI Si";
        TaskAddSub taskAddSub = new TaskAddSub(s1,s2);
        //新建两个进程
        Thread zhang, li;
        zhang = new Thread(taskAddSub);
        li = new Thread(taskAddSub);
        zhang.setName(s1);
        li.setName(s2);
        //进程进入排队，准备运行
        zhang.start();
        li.start();
    }
}
