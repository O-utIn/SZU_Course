package Part2;

class Left extends Thread
{
    int n = 0;
    public void run()
    {
        while(true)
        {
            n++;
            System.out.println(n+"Left");
            try
            {
                //等待一个随机时间
                sleep((int)(Math.random()*100));
            }
            catch(InterruptedException e) {}
        }
    }
}
