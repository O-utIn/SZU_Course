package Part3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.atomic.AtomicBoolean;

public class Bridge {

    private static final int allPeople = 20;
    private static final ReentrantLock bridgeLock = new ReentrantLock();
    private static int southCount = 0;
    private static int northCount = 0;
    private static AtomicBoolean southflag = new AtomicBoolean(false);
    private static AtomicBoolean northflag = new AtomicBoolean(false);
    private static AtomicBoolean finished = new AtomicBoolean(false);
    private static Random random = new Random();

    public static void main(String[] args) {
        int southWin = 0;
        int northWin = 0;

        for (int i = 0; i < 10; i++) {
            southCount = 0;
            northCount = 0;
            southflag.set(false);
            northflag.set(false);
            finished.set(false);
            List<Thread> threads = new ArrayList<>();

            // 每一个人创建一个线程
            for (int j = 0; j < allPeople; j++) {
                threads.add(new Thread(new Crosser("S" + (j + 1), true)));
                threads.add(new Thread(new Crosser("N" + (j + 1), false)));
            }

            // 打乱线程顺序
            Collections.shuffle(threads);

            // 启动所有线程
            for (Thread thread : threads) {
                try {
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

            // 每次循环输出先完成的一方
            if (southflag.get()) {
                System.out.println("第" + (i + 1) + "次运行，南边先完成过桥");
                southWin++;
            } else {
                System.out.println("第" + (i + 1) + "次运行，北边先完成过桥");
                northWin++;
            }
        }

        System.out.println("南边先完成过桥的次数：" + southWin);
        System.out.println("北边先完成过桥的次数：" + northWin);
    }

    static class Crosser implements Runnable {
        private final String name;
        private final boolean isSouth; // 作为一个判别flag

        Crosser(String name, boolean isSouth) {
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
                    if (southCount == allPeople && !finished.get()) {
                        southflag.set(true);
                        finished.set(true);
                    }
                } else {
                    northCount++;
                    if (northCount == allPeople && !finished.get()) {
                        northflag.set(true);
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