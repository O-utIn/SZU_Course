package Part2;

class TaskAddSub implements Runnable
{
    String s1,s2;
    TaskAddSub(String s1,String s2){this.s1=s1; this.s2=s2;}
    public void run(){
        int i=0; //局部变量
        while(true){
            if(Thread.currentThread().getName().equals(s1)){
                i=i+1; //局部变量
                System.out.println(s1 + ":" + i);
                //当进程1的i>=4
                if(i>=4){
                    System.out.println(s1 + "Finished");
                    return;
                }
            }
            else if(Thread.currentThread().getName().equals(s2)){
                i=i-1; //局部变量
                System.out.println(s2 + ":" + i);
                //当进程2的i<=-4
                if(i<=-4){
                    System.out.println(s2 + "Finished");
                    return;
                }
            }
            //等待800ms
            try{ Thread.sleep(800); }
            catch(InterruptedException e) {}
        }
    }
}