#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

// 声明 my_addr 变量
struct sockaddr_in my_addr;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    // 创建 UDP 套接字
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // 设置套接字选项
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR, (char *)&opt, sizeof(opt)) == SOCKET_ERROR) {
        printf("setsockopt failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 获取本地计算机的地址
    int addr_len = sizeof(my_addr);
    if (getsockname(sock, (struct sockaddr *)&my_addr, &addr_len) == SOCKET_ERROR) {
        printf("getsockname failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 构建 UDP Ping 请求报文
    char buf[1024];
    sprintf(buf, "UDP Ping request from %s at %ld", inet_ntoa(my_addr.sin_addr), time(NULL));

    // 发送 UDP Ping 请求报文
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    if(sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("sendto failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

// 等待和接收 UDP Ping 响应报文
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    int n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (n == SOCKET_ERROR) {
        printf("recvfrom failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 打印 UDP Ping 响应报文的内容
    printf("Received UDP Ping response from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Data: %s\n", buf);

    // 关闭套接字
    closesocket(sock);

    // 清理 Winsock
    WSACleanup();

    return 0;
}