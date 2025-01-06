package Part3;

class Account {
    private double money;

    public Account(double money) {
        this.money = money;
    }

    // 取款
    public synchronized void withdraw(double amount) {
        if (money >= amount) {
            money -= amount;
            System.out.println(Thread.currentThread().getName() + " 取款金额：" + amount + " 余额：" + money);
        }
        else {
            System.out.println(Thread.currentThread().getName() + " 余额不足，余额：" + money);
        }
    }

    // 存款
    public synchronized void deposit(double amount) {
        money += amount;
        System.out.println(Thread.currentThread().getName() + " 存款金额：" + amount + " 余额：" + money);
    }
}

// 操作类实现存取款
class Operation implements Runnable {
    private final Account account;
    private final double amount;

    public Operation(Account account, double amount) {
        this.account = account;
        this.amount = amount;
    }

    @Override
    public void run() {
        if (Thread.currentThread().getName().startsWith("取款")) {
            account.withdraw(amount);
        }
        else if (Thread.currentThread().getName().startsWith("存款")) {
            account.deposit(amount);
        }
    }
}

public class Bank {
    public static void main(String[] args) {
        Account account = new Account(100);

        // 创建取款线程
        Thread withdrawal1 = new Thread(new Operation(account, 10), "取款1");
        Thread withdrawal2 = new Thread(new Operation(account, 10), "取款2");
        Thread withdrawal3 = new Thread(new Operation(account, 10), "取款3");

        // 创建存款线程
        Thread save1 = new Thread(new Operation(account, 10), "存款1");
        Thread save2 = new Thread(new Operation(account, 10), "存款2");

        withdrawal1.start();
        withdrawal2.start();
        withdrawal3.start();
        save1.start();
        save2.start();
    }
}