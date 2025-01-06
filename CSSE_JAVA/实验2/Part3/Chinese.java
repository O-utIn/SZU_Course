package Part3;

public class Chinese extends Human{

    public Chinese(String name) {
        super(name);
    }

    @Override
    public String sayHello() {
        return "你好 " + name + " !";
    }

    public static void main(String[] args) {
        Human chinese=new Chinese("老王");
        System.out.println(chinese.sayHello());
    }
}
