package Part3;
import java.io.*;
import java.net.*;
import java.util.*;

public class ServerS {
    private static final int PORT = 9000;
    private static Set<PrintWriter> clientWriters = Collections.synchronizedSet(new HashSet<>());

    public static void main(String[] args) throws IOException {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Server is listening on port " + PORT);
            while (true) {
                Socket socket = serverSocket.accept();
                System.out.println("New client connected");
                new ClientHandler(socket).start();
            }
        } catch (IOException e) {
            System.err.println("Server exception: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private static class ClientHandler extends Thread {
        private Socket socket;
        private PrintWriter out;
        private BufferedReader in;

        public ClientHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            try {
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                out = new PrintWriter(socket.getOutputStream(), true);

                synchronized (clientWriters) {
                    clientWriters.add(out);
                }

                String message;
                while ((message = in.readLine()) != null) {
                    if ("exit".equalsIgnoreCase(message.trim())) break;
                    broadcastMessage("[" + socket.getInetAddress().getHostAddress() + "] " + message, out); // 传递当前的 PrintWriter
                }

                closeConnection();
            } catch (IOException e) {
                System.err.println("Exception from chat client: " + e.getMessage());
            } finally {
                closeConnection();
            }
        }



        private void broadcastMessage(String message, PrintWriter sender) {
            synchronized (clientWriters) {
                for (PrintWriter writer : clientWriters) {
                    if (writer != sender) {
                        writer.println(message);
                    }
                }
            }
        }

        private void closeConnection() {
            if (out != null) {
                synchronized (clientWriters) {
                    clientWriters.remove(out);
                }
            }
            try {
                if (in != null) in.close();
                if (out != null) out.close();
                if (socket != null) socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}