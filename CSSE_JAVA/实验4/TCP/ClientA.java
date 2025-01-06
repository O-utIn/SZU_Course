package Part3;


// ClientA.java
public class ClientA extends Client {
    public ClientA() {
        super("ClientA");
    }

    public static void main(String[] args) {
        new ClientA().startClient();
    }
}