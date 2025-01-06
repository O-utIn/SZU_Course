package Part3;

public class PostGraduateStudent {
    // 数据成员(学号、姓名、三门课程成绩)
    private String ID;
    private String name;
    private int math;
    private int programming;
    private int english;

    // 构造方法
    public PostGraduateStudent(String ID, String name, int math, int programming, int english)
    {
        this.ID = ID;
        this.name = name;
        this.math = math;
        this.programming = programming;
        this.english = english;
    }

    // 计算总分
    public int comSum()
    {
        return math + programming + english;
    }

    // 计算平均分
    public double comAvg()
    {
        return (double)(math+programming+english)/3;
    }

    // 计算最高分科目
    public int comMax()
    {
        int Max=math;
        if(programming>Max)
            Max=programming;
        if(english>Max)
            Max=english;
        return Max;
    }

    // 根据总分比较两个学生，返回总分高的学生姓名
    public String ComPare(PostGraduateStudent A)
    {
        int comSumA = A.comSum();
        if(comSumA>comSum())
            return A.name;
        else if(comSumA<comSum())
            return name;
        else return "Same";
    }

    // 将学生属性转换为字符串形式打印
    public String toString() {
        return "PostGraduateStudent{" +
                "ID='" + ID + '\'' +
                ", name='" + name + '\'' +
                ", math=" + math +
                ", programming=" + programming +
                ", english=" + english +
                ", 总分=" + comSum() +
                ", 平均分=" + comAvg() +
                ", 最高分=" + comMax() +
                '}';
    }

    public static void main(String[] args) {
        // 创建两个学生
        PostGraduateStudent student1 = new PostGraduateStudent("20220100001",
                "Alice", 90, 85, 95);
        PostGraduateStudent student2 = new PostGraduateStudent("20220100002",
                "Bob", 89, 92, 80);

        // 输出两个学生的信息
        System.out.println(student1);
        System.out.println(student2);

        // 比较两个学生的总分，输出总分高的学生姓名
        System.out.println("The outstanding is "+student1.ComPare(student2));
    }
}
