import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class WebDownload {
    public static void main(String[] args) {
        String urlStr = "http://www.szu.edu.cn";
        try {
            URL url = new URL(urlStr);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setInstanceFollowRedirects(false);
            int responseCode = connection.getResponseCode();

            if (responseCode == HttpURLConnection.HTTP_MOVED_TEMP || responseCode == HttpURLConnection.HTTP_MOVED_PERM) {
                String newUrl = connection.getHeaderField("Location");
                url = new URL(newUrl);
                connection = (HttpURLConnection) url.openConnection();
            }

            try (InputStream in = new BufferedInputStream(connection.getInputStream());
                 FileOutputStream fileOutputStream = new FileOutputStream("szu_homepage.html")) {

                byte[] dataBuffer = new byte[1024];
                int bytesRead;
                long totalBytes = 0;
                while ((bytesRead = in.read(dataBuffer, 0, 1024)) != -1) {
                    fileOutputStream.write(dataBuffer, 0, bytesRead);
                    totalBytes += bytesRead;
                }
                System.out.println("深圳大学首页已下载到 szu_homepage.html");
                System.out.println("下载的网页文件大小为: " + totalBytes + " 字节");
            }
        } catch (IOException e) {
            System.err.println("下载网页时发生错误: " + e.getMessage());
        }
    }
}