package Part3;

import java.io.*;
import java.net.*;
import java.util.Scanner;

public abstract class Client {
    protected static final String SERVER_HOST = "localhost";
    protected static final int SERVER_PORT = 9000;
    protected String nickname;

    public Client(String nickname) {
        this.nickname = nickname;
    }

    public void startClient() {
        Scanner scanner = new Scanner(System.in);
        try (Socket socket = new Socket(SERVER_HOST, SERVER_PORT);
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            // Start a thread to read messages from the server
            Thread receiveThread = new Thread(() -> {
                try {
                    String serverMessage;
                    while ((serverMessage = in.readLine()) != null) {
                        System.out.println(serverMessage);
                    }
                } catch (IOException e) {
                    System.err.println("Error receiving message from server.");
                }
            });
            receiveThread.start();

            // Send user input to the server
            System.out.println(nickname + " has joined the chat!");
            while (true) {
                String userInput = scanner.nextLine();
                if ("exit".equalsIgnoreCase(userInput.trim())) break;
                out.println("[" + nickname + "] " + userInput);
            }

        } catch (UnknownHostException e) {
            System.err.println("Don't know about host " + SERVER_HOST);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to " + SERVER_HOST);
        } finally {
            scanner.close();
        }
    }
}