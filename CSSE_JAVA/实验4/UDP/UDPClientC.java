package UDP;

import java.net.SocketException;

public class UDPClientC extends UDPClient {
    public UDPClientC() throws SocketException {
        super("ClientC");
    }

    public static void main(String[] args) {
        try {
            new UDPClientC().startClient();
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }
}