package UDP;

import java.io.*;
import java.net.*;
import java.util.Scanner;

public class UDPClient extends Thread {
    protected static final String SERVER_HOST = "localhost";
    protected static final int SERVER_PORT = 9001;
    private final String nickname;
    private final DatagramSocket socket;

    public UDPClient(String nickname) throws SocketException {
        this.nickname = nickname;
        this.socket = new DatagramSocket();

        // 自动发送注册消息给服务器
        registerWithServer();
        System.out.println(nickname + " has registered with the server.");
    }

    private void registerWithServer() {
        try {
            String registerMessage = "REGISTER:" + nickname;
            byte[] buffer = registerMessage.getBytes();
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length, InetAddress.getByName(SERVER_HOST), SERVER_PORT);
            socket.send(packet);
        } catch (IOException e) {
            System.err.println("Error registering with the server.");
            e.printStackTrace();
        }
    }

    public void startClient() {
        Scanner scanner = new Scanner(System.in);

        // 启动接收消息的线程
        Thread receiveThread = new Thread(this);
        receiveThread.start();

        System.out.println(nickname + " has joined the chat!");

        while (true) {
            String userInput = scanner.nextLine();
            if ("exit".equalsIgnoreCase(userInput.trim())) break;
            sendMessage("[" + nickname + "] " + userInput);
        }

        // 确保在退出前关闭资源
        try {
            socket.close();
            scanner.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void sendMessage(String message) {
        try {
            byte[] buffer = message.getBytes();
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length, InetAddress.getByName(SERVER_HOST), SERVER_PORT);
            socket.send(packet);
        } catch (IOException e) {
            System.err.println("Error sending message.");
        }
    }

    @Override
    public void run() {
        byte[] receiveBuffer = new byte[1024];
        DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);

        while (!socket.isClosed()) {
            try {
                socket.receive(receivePacket);
                String message = new String(receivePacket.getData(), 0, receivePacket.getLength()).trim();
                System.out.println(message);

                receivePacket.setLength(receiveBuffer.length);
            } catch (IOException e) {
                if (!socket.isClosed()) {
                    System.err.println("Error receiving message.");
                }
                break;
            }
        }
    }

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java UDPClient <nickname>");
            return;
        }

        String nickname = args[0];
        try {
            UDPClient client = new UDPClient(nickname);
            client.startClient();
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }
}