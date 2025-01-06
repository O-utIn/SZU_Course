package Example8_4;

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


class TaskBank implements Runnable
{
    private int money = 0; String name1,name2;
    TaskBank(String s1, String s2){ name1 = s1; name2 = s2; }
    public void setMoney(int mount){ money = mount; }
    public void run()
    {
        while(true)
        {
            //每次-10
            money = money-10;
            if(Thread.currentThread().getName().equals(name1)){
                System.out.println(name1 + ": " + money);
                //进程1的money<=100时结束
                if(money<=100){
                    System.out.println(name1 + ": Finished");
                    return;
                }
            }
            else
            if(Thread.currentThread().getName().equals(name2)){
                System.out.println(name2 + ": " + money);
                //进程2的money<=60时结束
                if(money<=60){
                    System.out.println(name2 + ": Finished");
                    return;
                }
            }
            //等待800ms
            try{ Thread.sleep(800); }
            catch(InterruptedException e) {}
        }
    }
}
