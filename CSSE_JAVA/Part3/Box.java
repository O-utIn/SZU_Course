package Part3;

public class Box {
    // 数据成员：长方体的长、宽和高
    private int length;
    private int width;
    private int height;

    public Box(int length, int width, int height)
    {
        this.length = length;
        this.width = width;
        this.height = height;
    }

    // 用于设置长、宽、高的值
    public void setInfo(int length,int width,int height)
    {
        this.length=length;
        this.width=width;
        this.height=height;
    }

    // 用于计算体积
    public int volume()
    {
        return length * width * height;
    }

    // 计算表面积
    public int area()
    {
        return 2 * (length * width + width * height + height * length);
    }

    // 将长方体的长、宽、高、体积和表面积转换为字符串并返回
    public String toString()
    {
        return "Box{" +
                "length=" + length +
                ",width=" + width +
                ",height=" + height +
                ",volume=" + volume() +
                ",area=" + area() +
                '}';
    }

    public static void main(String[] args) {
        Box B = new Box(10, 20, 30); // 创建一个Box对象，长宽高分别为10,20,30
        System.out.println(B.toString()); // 打印该Box对象的信息

        B.setInfo(20,10,20); //修改长宽高信息
        System.out.println("volume="+B.volume()); //体积
        System.out.println("area="+B.area()); //表面积
        System.out.println(B.toString()); // 打印更新后Box对象的信息
    }
}
