package Part3;

public class Spaniard extends Human {

    public Spaniard(String name) {
        super(name);
    }

    @Override
    public String sayHello() {
        return "Hola " + name + " !";
    }

    public static void main(String[] args) {
        Human spaniard = new Spaniard("Bob");
        System.out.println(spaniard.sayHello());
    }
}
