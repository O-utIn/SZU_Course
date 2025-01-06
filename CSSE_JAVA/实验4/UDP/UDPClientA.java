package UDP;

import java.net.SocketException;

public class UDPClientA extends UDPClient {
    public UDPClientA() throws SocketException {
        super("ClientA");
    }

    public static void main(String[] args) {
        try {
            new UDPClientA().startClient();
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }
}


