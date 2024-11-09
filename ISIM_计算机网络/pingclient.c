
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ws2tcpip.h>
#define SERVER_PORT 8888
#define MAX_PACKET_SIZE 64

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    char buf[MAX_PACKET_SIZE];
    struct timeval start, end;
    double rtt;
    int num_packets_sent = 0;
    int num_packets_received = 0;
    int min_rtt = INT_MAX;
    int max_rtt = INT_MIN;
    double avg_rtt = 0.0;

    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        perror("WSAStartup 失败");
        exit(EXIT_FAILURE);
    }

    // 创建一个 UDP 套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("创建套接字失败");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 发送数据报
    if (argc > 1) {
        strcpy(buf, argv[1]);
    } else {
        strcpy(buf, "ping");
    }
    if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &server_addr, server_len) < 0) {
        perror("发送数据报失败");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    num_packets_sent++;

    // 获取发送时间
    gettimeofday(&start, NULL);

    // 接收数据报
    while (num_packets_received < 4) {
        if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, &server_len) < 0) {
            perror("接收数据报失败");
            closesocket(sockfd);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // 获取接收时间
        gettimeofday(&end, NULL);

        // 计算 RTT
        rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

        // 更新统计信息
        num_packets_received++;
        if (rtt < min_rtt) {
            min_rtt = rtt;
        }
        if (rtt > max_rtt) {
            max_rtt = rtt;
        }
        avg_rtt += rtt;

        // 打印数据报和 RTT
        printf("来自 %s 的回复: 字节=%d 时间=%.1fms \n", inet_ntoa(server_addr.sin_addr), strlen(buf), rtt);

        // 重新发送数据报
        if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &server_addr, server_len) < 0) {
            perror("发送数据报失败");
            closesocket(sockfd);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        num_packets_sent++;

        // 获取发送时间
        gettimeofday(&start, NULL);
    }

    // 关闭套接字
    closesocket(sockfd);
    
    // 清理 Winsock
    WSACleanup();

    // 打印 ping 统计信息
    printf("\n%s 的 Ping 统计信息:\n",inet_ntoa(server_addr.sin_addr));
    printf("    数据包: 已发送 = %d，已接收 = %d，丢失 = %d (%.1f%% 丢失)\n", num_packets_sent, num_packets_received, num_packets_sent - num_packets_received, (double)(num_packets_sent - num_packets_received) / num_packets_sent * 100.0);
    printf("往返行程的估计时间(以毫秒为单位):\n");
    printf("    最短 = %.1fms，最长 = %.1fms，平均 = %.1fms\n", min_rtt, max_rtt, avg_rtt / num_packets_received);

    return 0;
}
