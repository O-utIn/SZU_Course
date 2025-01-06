package Part2;

import java.util.Arrays;

public class MinNum {
    public static void main(String[] args)
    {
		float[][][] arr = new float [1000][1000][100]; //创建一个1000×1000×100三维的float数组
        for(int i=0;i<1000;i++)
        {
            for(int j=0;j<1000;j++)
            {
                for(int k=0;k<100;k++)
                    arr[i][j][k]=(float)Math.random();  //使用Math.random()生成0-1之间的数,对数组中的元素进行随机赋值
            }
        }

        // 创建一个15个数的数组，用于存储结果
        float[] minNum = new float[15];
        for(int i=0;i<15;i++)
            minNum[i]=1; // 数组初始化为1

        // 记录程序开始时间
        long startTime = System.currentTimeMillis();

        // 遍历三维数组以找到最小的15个数
        for (int i = 0; i < 1000; i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                for (int k = 0; k < 100; k++)
                {
                    float current = arr[i][j][k];

                    // 如果当前数小于15个数中的某个数，则替换
                    for (int m = 0; m < 15; m++)
                    {
                        if (current < minNum[m])
                        {
                            // 向后移动比当前数大的数
                            for (int n = m; n < 14; n++)
                                minNum[n + 1] = minNum[n];
                            // 插入当前的数
                            minNum[m] = current;
                            break; // 退出内层循环
                        }
                    }
                }
            }
        }


        // 记录程序结束时间并计算总时间
        long endTime = System.currentTimeMillis();
        System.out.println("程序运行时间为: " + (endTime - startTime) + " ms");

        // 输出最小的15个数
        System.out.println("最小的15个数是:");
        for (float num : minNum)
            System.out.println(num);

    }
}
