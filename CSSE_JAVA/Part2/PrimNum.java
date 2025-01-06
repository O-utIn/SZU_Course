package Part2;

public class PrimNum {
    public static void main(String[] args) {
        int count = 0; // 初始化计数器
        System.out.println("10到10000之间的素数有：");
        for (int number = 10; number <= 10000; number++) {
            if (isPrime(number)) {
                System.out.print(number + " "); //输出找到的素数
                count++; //更新计数器
                if(count % 30 == 0)
                    System.out.println();
            }
        }
        System.out.println("\n一共有：" + count);
    }

    public static boolean isPrime(int num) {
        for (int i = 2; i * i <= num; i++) { // 如果能被2到根号num之间的一个数整除，则不是素数
            if (num % i == 0) {
                return false;
            }
        }
        return true; // 如果没有找到能整除的数，则是素数
    }
}
