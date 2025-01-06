package Part3;

// ChatRoomLauncher.java
public class ChatRoomLauncher {
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: java ChatRoomLauncher <clientType> <nickname>");
            return;
        }

        String clientType = args[0];
        String nickname = args[1];

        Client client;
        switch (clientType.toLowerCase()) {
            case "a":
                client = new ClientA();
                break;
            case "b":
                client = new ClientB();
                break;
            case "c":
                client = new ClientC();
                break;
            default:
                System.out.println("Unknown client type: " + clientType);
                return;
        }

        client.startClient();
    }
}