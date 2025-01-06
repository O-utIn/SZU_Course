package Part3;

class PrintNum implements Runnable {
    // 共享数据对象
    private final Share data;

    public PrintNum(Share data) {
        this.data = data;
    }

    // 用于生成随机数并添加到共享数据
    public synchronized void generateNums() {
        PrintNum random = new PrintNum(data);
        while (true) {
            float randomNum = (float) Math.random();
            System.out.println(randomNum);

            // 将生成的随机数放入共享数据，供统计线程使用
            data.addNum(randomNum);
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void run() {
        generateNums();
    }

}
