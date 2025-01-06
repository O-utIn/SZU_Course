package Part3.Ex;

public class Spaniard implements Human{
    public String name;

    public Spaniard(String name){
        this.name = name;
    }

    public String sayHello() {
        return "Hola " + name;
    }
}
