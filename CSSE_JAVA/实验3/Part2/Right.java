package Part2;

class Right extends Thread
{
    int n = 0;
    public void run()
    {
        while(true)
        {
            n++;
            System.out.println(n+"Right");
            try
            {
                sleep((int)(Math.random()*100));
            }
            catch(InterruptedException e){}
        }
    }
}
