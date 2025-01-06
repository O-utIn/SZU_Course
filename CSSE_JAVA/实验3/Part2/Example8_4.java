package Part2;

public class Example8_4
{
    public static void main(String args[])
    {
        String s1="treasurer zhang"; // 会计
        String s2="cashier cheng"; // 出纳
        TaskBank taskBank = new TaskBank(s1,s2);
        //设置money为120
        taskBank.setMoney(120);
        Thread zhang;
        Thread cheng;
        zhang = new Thread(taskBank); // 目标对象bank
        cheng = new Thread(taskBank); // 目标对象bank
        //设置两个进程名称
        zhang.setName(s1);
        cheng.setName(s2);
        //进程进入排队
        zhang.start();
        cheng.start();
    }
}