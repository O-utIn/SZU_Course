package Part2;

public class TaskThreadDemo {
    public static void main(String[] args) {
        // 创建任务
        Runnable printA = new PrintChar('a', 100);
        Runnable printB = new PrintChar('b', 100);
        Runnable print100 = new PrintNum(100);

        // 创建线程
        Thread thread1 = new Thread(printA);
        Thread thread2 = new Thread(printB);
        Thread thread3 = new Thread(print100);

        // 启动线程
        thread1.start();
        thread2.start();
        thread3.start();
    }
}
