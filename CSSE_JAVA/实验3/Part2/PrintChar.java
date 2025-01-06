package Part2;

// 打印指定次数字符的任务类
class PrintChar implements Runnable {
    private char charToPrint; // 要打印的字符
    private int times; // 重复打印的次数

    // 构造一个带有指定字符和打印次数的任务
    public PrintChar(char c, int t) {
        charToPrint = c;
        times = t;
    }

    @Override
    // 重写 run()方法以告知系统要执行的任务
    public void run() {
        for (int i = 0; i < times; i++) {
            System.out.print(charToPrint);
        }
    }
}
