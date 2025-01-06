package Part3;

public class Italian extends Human{

    public Italian(String name) {
        super(name);
    }

    @Override
    public String  sayHello() {
        return "Ciao " + name +" !";
    }

    public static void main(String[] args) {
        Human italian = new Italian("Tony");
        System.out.println(italian.sayHello());
    }
}
