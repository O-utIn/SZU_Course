import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;

public class LocalIP {
    public static void main(String[] args) {
        try {
            // 获取并显示本机名称
            InetAddress localHost = InetAddress.getLocalHost();
            System.out.println("本地机名称: " + localHost.getHostName());

            // 遍历网络接口获取 IP 地址
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface ni = interfaces.nextElement();
                Enumeration<InetAddress> addresses = ni.getInetAddresses();
                while (addresses.hasMoreElements()) {
                    InetAddress addr = addresses.nextElement();
                    // 过滤回环地址、链路本地地址，只保留 IPv4 地址
                    if (!addr.isLoopbackAddress() && !addr.isLinkLocalAddress() && addr instanceof java.net.Inet4Address) {
                        System.out.println("IP 地址: " + addr.getHostAddress() + " (接口: " + ni.getDisplayName() + ")");
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}