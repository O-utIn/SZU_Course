package Part1;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.atomic.AtomicBoolean;

public class Bridge {

    private static final int PEOPLE_COUNT = 20;
    private static final ReentrantLock bridgeLock = new ReentrantLock();
    private static int southCount = 0;
    private static int northCount = 0;
    private static AtomicBoolean southFlag = new AtomicBoolean(false);
    private static AtomicBoolean northFlag = new AtomicBoolean(false);
    private static AtomicBoolean finished = new AtomicBoolean(false);
    private static Random random = new Random();

    public static void main(String[] args) {
        int southWins = 0;
        int northWins = 0;

        for (int i = 0; i < 10; i++) {
            southCount = 0;
            northCount = 0;
            southFlag.set(false);
            northFlag.set(false);
            finished.set(false);
            List<Thread> threads = new ArrayList<>();

            // 创建所有线程
            for (int j = 0; j < PEOPLE_COUNT; j++) {
                threads.add(new Thread(new BridgeCrosser("S" + (j + 1), true)));
                threads.add(new Thread(new BridgeCrosser("N" + (j + 1), false)));
            }

            // 打乱线程顺序
            Collections.shuffle(threads);

            // 启动所有线程
            for (Thread thread : threads) {
                try {
                    // 引入随机延迟以增加随机性
                    Thread.sleep(random.nextInt(100));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                thread.start();
            }

            // 等待所有线程结束
            for (Thread thread : threads) {
                try {
                    thread.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            // 输出哪边先完成
            if (southFlag.get()) {
                System.out.println("第" + (i + 1) + "次运行，南边先完成过桥");
                southWins++;
            } else {
                System.out.println("第" + (i + 1) + "次运行，北边先完成过桥");
                northWins++;
            }
        }

        // 输出最终结果
        System.out.println("南边先完成过桥的次数：" + southWins);
        System.out.println("北边先完成过桥的次数：" + northWins);
    }

    static class BridgeCrosser implements Runnable {
        private final String name;
        private final boolean isSouth;

        BridgeCrosser(String name, boolean isSouth) {
            this.name = name;
            this.isSouth = isSouth;
        }

        @Override
        public void run() {
            crossBridge();
        }

        private void crossBridge() {
            bridgeLock.lock();
            try {
                System.out.print(name);
                if (!name.equals("S20") && !name.equals("N20")) {
                    System.out.print(", ");
                } else {
                    System.out.print(", ");
                }
                Thread.sleep(10); // 模拟过桥所需的时间
                if (isSouth) {
                    southCount++;
                    if (southCount == PEOPLE_COUNT && !finished.get()) {
                        southFlag.set(true);
                        finished.set(true);
                    }
                } else {
                    northCount++;
                    if (northCount == PEOPLE_COUNT && !finished.get()) {
                        northFlag.set(true);
                        finished.set(true);
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                bridgeLock.unlock();
            }
        }
    }
}