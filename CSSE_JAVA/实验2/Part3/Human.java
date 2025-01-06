package Part3;

public abstract class Human {
    public String name;

    public Human(String name)
    {
        this.name = name;
    }

    public abstract String sayHello(); //抽象方法
}
