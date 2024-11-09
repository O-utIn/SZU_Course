#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <ws2tcpip.h>

#define SERVER_PORT 8888

int main()
{
    WSADATA wsaData;
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    char buf[256];

    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
        perror("WSA启动失败");
        exit(EXIT_FAILURE);
    }

    // 创建一个 UDP 套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
        perror("创建套接字失败");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 绑定套接字到服务器地址
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
        perror("绑定套接字失败");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    printf("服务器已启动\n");
    

    // 接收来自客户端的数据报
    while (1)
	{
        memset(buf, 0, sizeof(buf));
        if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, &server_len) < 0)
		{
            perror("接收数据报失败");
            closesocket(sockfd);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // 打印客户端报文
        printf("来自客户端的报文：%s\n", buf);

        // 发送响应
        if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &server_addr, server_len) < 0)
		{
            perror("发送数据报失败");
        }
    }

    // 关闭套接字
    closesocket(sockfd);
    
    // 清理 Winsock
    WSACleanup();

    return 0;
}
