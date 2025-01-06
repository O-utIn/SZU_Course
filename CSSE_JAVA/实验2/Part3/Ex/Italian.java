package Part3.Ex;

public class Italian implements Human {
    public String name;

    public Italian(String name) {
        this.name=name;
    }

    public String sayHello() {
        return "Ciao "+name;
    }
}
