package Part2;

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
