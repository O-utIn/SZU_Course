package Part3;

public class Teacher {
    //成员数据
    private String name;
    private String title;
    private String course;
    private String research;
    private String office;

    //含参构造方法
    public Teacher(String name, String title, String course, String research, String office)
    {
        this.name = name;
        this.title = title;
        this.course = course;
        this.research = research;
        this.office = office;
    }

    public String getName() //获取姓名
    {
        return "Name is " + name;
    }

    public String getTitle() //获取职位
    {
        return "Title is " + title;
    }

    public String getCourse() //主讲课程
    {
        return "Course is "+ course;
    }

    public String getResearch() //研究方向
    {
        return "Research is about " + research;
    }

    public String getOffice() //办公室
    {
        return "Office is in "+office;
    }

    //转换为字符串用于打印信息
    @Override
    public String toString()
    {
        return "Teacher{" +
                "name:" + name +
                ", title:" + title +
                ", course is " + course +
                ", reserch is about " + research +
                ", office is in " + office +
                "}";
    }

    //在Teacher类内的main方法里面，创建该类的一个对象，并调用各个方法，
    public static void main(String[] args) {
        Teacher TA = new Teacher("Alice","PhD",
                "math","AI","L9-999"); //创建一个Teacher
        System.out.println(TA); //打印全部信息

        System.out.println(TA.getName()); //打印姓名
        System.out.println(TA.getTitle()); //打印职位
        System.out.println(TA.getCourse()); //打印主讲课程
        System.out.println(TA.getResearch()); //打印研究方向
        System.out.println(TA.getOffice()); //打印办公室
    }

}

class ExP {
    //在Teacher类外的main方法里面，创建该类的一个对象，并调用各个方法
    public static void main(String[] args) {
        Teacher TB = new Teacher("Bob","leader",
                "english","math","L3-308"); //创建一个Teacher对象
        System.out.println(TB); //打印全部信息

        System.out.println(TB.getName()); //打印姓名
        System.out.println(TB.getTitle()); //打印职位
        System.out.println(TB.getCourse()); //打印主讲课程
        System.out.println(TB.getResearch()); //打印研究方向
        System.out.println(TB.getOffice()); //打印办公室
    }
}

