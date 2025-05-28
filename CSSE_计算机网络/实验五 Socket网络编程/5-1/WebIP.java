import java.net.InetAddress;
import java.net.UnknownHostException;

public class WebIP {
    public static void main(String[] args) {
        try {
            InetAddress[] addresses = InetAddress.getAllByName("www.csdn.net");
            System.out.println("网站 www.csdn.net 的IP地址如下：");
            for (InetAddress address : addresses) {
                System.out.println(address.getHostAddress());
            }
        } catch (UnknownHostException e) {
            System.err.println("无法获取网站的IP地址: " + e.getMessage());
        }
    }
}
