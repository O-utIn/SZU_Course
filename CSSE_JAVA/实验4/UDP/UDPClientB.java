package UDP;

import java.net.SocketException;

public class UDPClientB extends UDPClient {
    public UDPClientB() throws SocketException {
        super("ClientB");
    }

    public static void main(String[] args) {
        try {
            new UDPClientB().startClient();
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }
}