package Part1;

public class Queue {
    private double []element;
    private int size;

    //构造方法
    public Queue() {
        element=new double[32];
        size=0;
    }

    //向队列中加一个double数据，队列中已有的数据向后移动
    public void enQueue(double v) {
        //如果队列还没满32个元素
        if(size<32) {
            size++;
            for (int i = size - 1; i > 0; i--) {
                element[i] = element[i - 1];
            }
            element[0] = v;
        }
        //如果队列满了32个元素
        else if(size==32) {
            for(int i=31; i>0; i--) {
                element[i] = element[i-1];
            }
            element[0] = v;
        }
        //其他错误
        else {
            System.out.println("Other error!");
        }
    }

    //删除并返回队列头元素
    public  double deQueue() {
        return element[--size];
    }

    //返回队列第一个元素
    public double getHead() {
        return element[size-1];
    }

    //返回队列最后一个元素
    public double getTail() {
        return element[0];
    }

    //判断队列是否空
    public boolean isEmpty() {
        return size==0;
    }

    //判断队列是否满
    public boolean isFull() {
        return size==32;
    }

    //返回队列大小
    public int getSize() {
        return size;
    }

    public static void main(String[] args) {
        Queue q=new Queue();
        //向队列填充32个数据
        for(int i=0;i<32;i++) {
            q.enQueue(i);
        }
        System.out.println("Size is "+q.getSize()); //目前的元素个数（31-0）：32个
        System.out.println("Delete the head is "+q.deQueue()); //删除队列头0，SIZE:31
        q.enQueue(32); //向队列尾新添元素32（32 31...2 1）SIZE:32
        q.enQueue(33); //向队列尾新添元素33（33 32...3 2）SIZE:32
        System.out.println("Delete the head is "+q.deQueue()); //删除队列头2，SIZE:31
        System.out.println("The head element is "+q.getHead()); //返回队列头元素3，SIZE:31
        System.out.println("The tail element is "+q.getTail()); //返回队列尾33，SIZE:31
        System.out.println("Empty?:"+q.isEmpty()); //判断队列是否空 SIZE:31
        System.out.println("Full?:"+q.isFull()); //判断队列是否满 SIZE:31
        q.enQueue(34); //向队列尾新添元素34（34 33...2 1）SIZE:32
        System.out.println("Full?:"+q.isFull()); //判断队列是否满 SIZE:32
        System.out.println("Size is "+q.getSize()); //返回队列元素个数：32
    }

}
