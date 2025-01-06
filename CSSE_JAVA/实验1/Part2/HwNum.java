package Part2;
import java.math.BigInteger;
import java.util.Random;

public class HwNum {
    public static void main(String[] args) {
        Random random = new Random(); // 创建Random对象以生成随机数

        for (int i = 0; i < 5; i++)
        {
            // 创建一个21位的正数
            BigInteger num = new BigInteger(1, new byte[20]).setBit(20 * 8);
            //界定21位数的范围
            BigInteger min = new BigInteger("100000000000000000000"); // 21位的最小值
            BigInteger max = new BigInteger("999999999999999999999"); // 21位的最大值
            //生成数字
            num = min.add(new BigInteger(String.valueOf(random.nextLong()))
                    .mod(max.subtract(min).add(BigInteger.ONE)));

            // 打印这五个随机数
            System.out.print("随机数: " + num + ", ");

            // 计算它们的逆序数
            String numStr = num.toString();
            StringBuilder stnum = new StringBuilder(numStr);
            String reversedStr = stnum.reverse().toString();
            BigInteger Rnum = new BigInteger(reversedStr);

            // 打印逆序数
            System.out.print("逆序数: " + Rnum + ", ");

            // 判断是否为回文并打印结果
            if (num.equals(Rnum))
            {
                System.out.println("是回文。");
            }
            else
            {
                System.out.println("不是回文。");
            }
        }
    }
}
