package Part2;

import java.util.Scanner; //导入输入流

public class ReverPart //主类
{
    public static void main(String[] args) //函数入口
    {
        Scanner sca=new Scanner(System.in); //设置输入流

        float arr[] = new float [21]; //创建一个数组
        for(int i=0;i<21;i++)
            arr[i]=sca.nextFloat();  //从键盘输入21个浮点数放入数组

        for(int i=0;i<21;i++) //输出未置换位置前的数组
            System.out.print(arr[i]+" ");
        System.out.println();


        float temp=0;  //设置一个中间元素用于置换元素
        for(int j=0;j<5;j++) //对5对元素进行位置置换
        {
            temp=arr[j];  //第1个元素与第21个元素置换位置
            arr[j]=arr[20-j]; //第2个元素与第20个元素置换位置
            arr[20-j]=temp;  //以此类推，完成5对元素的置换
        }

        for(int i=0;i<21;i++)  //输出置换过位置后的数组
            System.out.print(arr[i]+" ");
    }
}
