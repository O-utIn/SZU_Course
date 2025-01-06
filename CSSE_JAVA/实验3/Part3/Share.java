package Part3;

// 共享数据类，用于线程间实现通信
class Share {
    private final float[] numbers = new float[5];
    private int count = 0;
    private int index = 0;

    // 同步方法，用于向共享数据结构中添加随机数
    public synchronized void addNum(float number) {
        numbers[index] = number;
        // 更新索引位置，实现循环利用数组
        index = (index + 1) % 5;
        count++;
        notifyAll();
    }

    // 同步方法，用于获取最近的5个随机数
    public synchronized float[] getNums() {
        while (count % 5 != 0) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // 创建一个新数组，用于返回最近的5个随机数
        float[] result = new float[5];
        System.arraycopy(numbers, 0, result, 0, 5);
        return result;
    }

    public int getCount() {
        return count;
    }
}
