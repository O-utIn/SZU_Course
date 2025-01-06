package Part2;

public class Example8_5 {
    public static void main(String args[]) {
        Task taskAdd = new Task();
        taskAdd.setNumber(10);
        Task taskSub = new Task();
        taskSub.setNumber(-10);
        Thread threadA, threadB, threadC, threadD;
        threadA = new Thread(taskAdd);
        threadB = new Thread(taskAdd);
        threadA.setName("add");
        threadB.setName("add");
        threadC = new Thread(taskSub);
        threadD = new Thread(taskSub);
        threadC.setName("sub");
        threadD.setName("sub");
        threadA.start();
        threadB.start();
        threadC.start();
        threadD.start();
    }
}