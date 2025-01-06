package UDP;
import java.io.IOException;
import java.net.*;
import java.util.*;

public class UDPServer {
    private static final int PORT = 9001;
    private static Set<String> clientAddresses = Collections.synchronizedSet(new HashSet<>());
    private static DatagramSocket socket;

    public static void main(String[] args) {
        try {
            // 初始化聊天室成员
            socket = new DatagramSocket(PORT);
            System.out.println("Server is listening on port " + PORT);

            byte[] receiveBuffer = new byte[1024];
            while (true) {
                DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);
                socket.receive(receivePacket);

                String message = new String(receivePacket.getData(), 0, receivePacket.getLength()).trim();

                InetAddress clientAddress = receivePacket.getAddress();
                int clientPort = receivePacket.getPort();

                // 用户连接注册消息
                if ("REGISTER".equalsIgnoreCase(message.trim())) {
                    synchronized (clientAddresses) {
                        clientAddresses.add(clientAddress.getHostAddress() + ":" + clientPort);
                    }
                    System.out.println("Client registered: " + clientAddress.getHostAddress() + ":" + clientPort);
                    continue;
                }

                // 添加信息发送者的地址
                synchronized (clientAddresses) {
                    clientAddresses.add(clientAddress.getHostAddress() + ":" + clientPort);
                }

                System.out.println("Received: " + message + " from " + clientAddress.getHostAddress() + ":" + clientPort);

                // 广播信息
                broadcastMessage(message, clientAddress, clientPort);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
        }
    }

    private static void broadcastMessage(String message, InetAddress senderAddress, int senderPort) throws IOException {
        synchronized (clientAddresses) {
            for (String addressPort : clientAddresses) {
                String[] parts = addressPort.split(":");
                InetAddress address = InetAddress.getByName(parts[0]);
                int port = Integer.parseInt(parts[1]);

                if (!(address.equals(senderAddress) && port == senderPort)) {
                    byte[] sendBuffer = message.getBytes();
                    DatagramPacket sendPacket = new DatagramPacket(sendBuffer, sendBuffer.length, address, port);
                    socket.send(sendPacket);
                }
            }
        }
    }
}