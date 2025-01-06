package Part2;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ExecutorDemo {
    public static void main(String[] args) {
        // 创建一个最大线程数为 3 的固定线程池
        ExecutorService executor = Executors.newFixedThreadPool(3);

        // 向执行器提交可运行任务
        executor.execute(new PrintChar('a', 100));
        executor.execute(new PrintChar('b', 100));
        executor.execute(new PrintNum(100));

        // 关闭执行器
        executor.shutdown();
    }
}
