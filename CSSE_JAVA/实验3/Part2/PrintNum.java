package Part2;

// 打印从 1 到给定数字 n 的任务类
class PrintNum implements Runnable {
    private int lastNum;

    // 构造一个用于打印从 1 到 n 的任务
    public PrintNum(int n) {
        lastNum = n;
    }

    @Override
    // 告知线程如何运行
    public void run() {
        for (int i = 1; i <= lastNum; i++) {
            System.out.print("" + i);
        }
    }
}
