package Part3.Ex;

public class Chinese implements Human{
    public String name;

    public Chinese(String name){
        this.name = name;
    }

    public String sayHello() {
        return "你好 " + name;
    }
}
