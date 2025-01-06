package Part3;

class Calculate implements Runnable {
    private final Share data;

    public Calculate(Share share) {
        this.data = share;
    }

    public void run() {
        while (true) {
            // 当共享数据中的计数是5的倍数且大于0时，进行计算
            if (data.getCount() % 5 == 0 && data.getCount() > 0) {
                // 获取最近的5个随机数
                float[] numbers = data.getNums();
                // 计算这5个数的和
                float sum = 0;
                for (float num : numbers) {
                    sum += num;
                }
                // 计算这5个随机数的平均值
                float average = sum / numbers.length;

                // 输出这5个随机数的和与平均值
                System.out.println("Sum: " + sum + "; Average:" + average);
            }

            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
