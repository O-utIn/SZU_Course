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

    // ��ʼ�� Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
        perror("WSA����ʧ��");
        exit(EXIT_FAILURE);
    }

    // ����һ�� UDP �׽���
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
        perror("�����׽���ʧ��");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // ���÷�������ַ
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // ���׽��ֵ���������ַ
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
        perror("���׽���ʧ��");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    printf("������������\n");
    

    // �������Կͻ��˵����ݱ�
    while (1)
	{
        memset(buf, 0, sizeof(buf));
        if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, &server_len) < 0)
		{
            perror("�������ݱ�ʧ��");
            closesocket(sockfd);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // ��ӡ�ͻ��˱���
        printf("���Կͻ��˵ı��ģ�%s\n", buf);

        // ������Ӧ
        if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &server_addr, server_len) < 0)
		{
            perror("�������ݱ�ʧ��");
        }
    }

    // �ر��׽���
    closesocket(sockfd);
    
    // ���� Winsock
    WSACleanup();

    return 0;
}
