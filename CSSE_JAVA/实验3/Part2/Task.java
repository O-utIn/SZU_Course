package Part2;

class Task implements Runnable
{
    private int number = 0;
    public void setNumber(int n)
    {
        number = n;
    }

    // 同步的增加方法
    public synchronized void incrementNumber() {
        number++;
        System.out.printf("%d\n", number);
    }

    // 同步的减少方法
    public synchronized void decrementNumber() {
        number--;
        System.out.printf("%12d\n", number);
    }

    public void run() {
        while (true) {
            if (Thread.currentThread().getName().equals("add")) {
                incrementNumber();
            }
            else if (Thread.currentThread().getName().equals("sub")) {
                decrementNumber();
            }
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
            }
        }
    }

}
