import java.io.*;
import java.net.*;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Server {
    private static final int SERVER_PORT = 9898; // Change to desired port
    private static final String REMOTE_IP = "192.168.43.44"; //"192.168.43.244";
    private static final int REMOTE_PORT = 9999;

    private BlockingQueue<String> queue = new LinkedBlockingQueue<>();

    public void start() throws IOException {
        try (ServerSocket serverSocket = new ServerSocket(SERVER_PORT)) {
            System.out.println("Server started on port " + SERVER_PORT);
            
            // Start the thread that sends data to the remote server
            new Thread(() -> sendDataToRemote()).start();

            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("New client connected: " + clientSocket.getInetAddress().getHostAddress());
                
                // Handle each client in a separate thread
                new Thread(() -> handleClient(clientSocket)).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void handleClient(Socket clientSocket) {
        try (BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
             PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)) {
             
            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                // Add the received data to the queue
                queue.add(inputLine);
                out.println("Received: " + inputLine);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void sendDataToRemote() {
        boolean connected = false;
        while (!connected) {
            try (Socket socket = new Socket(REMOTE_IP, REMOTE_PORT)) {
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                out.println("f2aabab2-d5db-4be2-b9bc-19cc6a16840e");
                connected = true; // Successfully connected
    
                while (true) {
                    String data = queue.take(); // Blocks until data is available
                    out.println(data);
                }
            } catch (IOException e) {
                e.printStackTrace();
                connected = false; // Connection failed, try again
                try {
                    Thread.sleep(5000); // Wait for 5 seconds before trying to reconnect
                } catch (InterruptedException ie) {
                    Thread.currentThread().interrupt();
                }
            } catch (InterruptedException ie) {
                Thread.currentThread().interrupt();
            }
        }
    }
    

    public static void main(String[] args) throws IOException {
        new Server().start();
    }
}
