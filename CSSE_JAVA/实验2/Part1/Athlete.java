package Part1;

import java.security.AlgorithmConstraints;

public class Athlete {
    public String name; //姓名
    public String gender; //性别
    public int age; //年龄
    public Item item; //项目
    public int medal; //奖牌数量

    //构造方法
    protected Athlete(String name,String gender,int age,Item item,int medal) {
        this.name=name;
        this.gender=gender;
        this.age=age;
        this.item=item;
        this.medal=medal;
    }

    //修改成员数据
    public void setInfo(String name,String gender,int age,Item item,int medal) {
        this.name=name;
        this.gender=gender;
        this.age=age;
        this.item=item;
        this.medal=medal;
    }

    //获取姓名
    public String getName() {
        return this.name;
    }

    public String getGender() {
        return this.gender;
    }

    //获取年龄
    public int getAge() {
        return age;
    }

    public String getItem() {
        return item.name;
    }

    //获取奖牌数量
    public int getMedal() {
        return medal;
    }

    public String toString()
    {
        return "Name:"+this.getName()+"\n"+
                "Gender:"+this.getGender()+"\n"+
                "Age:"+this.getAge()+"\n"+
                "Item:"+this.getItem()+"\n"+
                "The count of medal:"+this.getMedal()+"\n";
    }

    public static void main(String[] args) {
        Athlete medal1 = new Athlete("黄雨婷，盛李豪","female/male",18,
                new Item("Shoot","TargetRange",8,"Gun",100,8,8),
                4);
        System.out.println(medal1.toString());

        Athlete medal2 = new Athlete("陈艺文,昌雅妮","female",18,
                new Item("Diving","Pool",15,"None",120,16,3),
                3);
        System.out.println(medal2.toString());

        Athlete medal3 = new Athlete("谢瑜","male",18,
                new Item("Shoot","TargetRange",8,"Gun",100,8,8),
                1);
        System.out.println(medal3.toString());
    }
}
