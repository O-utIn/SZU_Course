package Part3;


public class Main {
    public static void main(String[] args) {
        // 创建共享数据对象
        Share share = new Share();

        Thread randomNum = new Thread(new PrintNum(share));
        Thread cal = new Thread(new Calculate(share));

        randomNum.start();
        cal.start();
    }
}