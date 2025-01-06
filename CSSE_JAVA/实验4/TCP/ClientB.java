package Part3;

// ClientB.java
public class ClientB extends Client {
    public ClientB() {
        super("ClientB");
    }

    public static void main(String[] args) {
        new ClientB().startClient();
    }
}