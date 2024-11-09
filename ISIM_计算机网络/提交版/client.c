#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ws2tcpip.h>

#define SERVER_PORT 8888
#define MAX_PACKET_SIZE 64

int main(int argc, char *argv[])
{
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
    char input[MAX_PACKET_SIZE];

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
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // ������ȡ�û����벢���͵�������
    while (1)
	{
        // ��ȡ�û�����
        printf("����Ҫ���͵��������ı��ģ������� \"ping\" ���� ping ����������ping���������������رգ���");
        gets(input);

        // ����û����� "ping"����ִ�� ping ����
        if (strcmp(input, "ping") == 0) {
            // �������ݱ�
            strcpy(buf, "ping");
            if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &server_addr, server_len) < 0)
			{
                perror("�������ݱ�ʧ��");
                closesocket(sockfd);
                WSACleanup();
                exit(EXIT_FAILURE);
            }

            num_packets_sent++;

            // ��ȡ����ʱ��
            gettimeofday(&start, NULL);

            // �������ݱ�
            while (num_packets_received < 4) {
                if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, &server_len) < 0)
				{
                    perror("�������ݱ�ʧ��");
                    closesocket(sockfd);
                    WSACleanup();
                    exit(EXIT_FAILURE);
                }

                // ��ȡ����ʱ��
                gettimeofday(&end, NULL);

                // ���� RTT
                rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec- start.tv_usec) / 1000.0;

                // ����ͳ����Ϣ
                num_packets_received++;
                if (rtt < min_rtt)
				{
                    min_rtt = rtt;
                }
                if (rtt > max_rtt)
				{
                    max_rtt = rtt;
                }
                avg_rtt += rtt;

                // ��ӡ���ݱ��� RTT
                printf("���� %s �Ļظ�: �ֽ�=%d ʱ��=%.1fms \n", inet_ntoa(server_addr.sin_addr), strlen(buf), rtt);

                // ���·������ݱ�
                if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &server_addr, server_len) < 0)
				{
                    perror("�������ݱ�ʧ��");
                    closesocket(sockfd);
                    WSACleanup();
                    exit(EXIT_FAILURE);
                }

                num_packets_sent++;

                // ��ȡ����ʱ��
                gettimeofday(&start, NULL);
            }

            // �ر��׽���
            closesocket(sockfd);
    
            // ���� Winsock
            WSACleanup();

            // ��ӡ ping ͳ����Ϣ
            printf("\n%s �� Ping ͳ����Ϣ:\n",inet_ntoa(server_addr.sin_addr));
            printf("    ���ݰ�: �ѷ��� = %d���ѽ��� = %d����ʧ = %d (%.1f%% ��ʧ)\n", num_packets_sent, num_packets_received, num_packets_sent - num_packets_received, (double)(num_packets_sent - num_packets_received) / num_packets_sent * 100.0);
            printf("�����г̵Ĺ���ʱ��(�Ժ���Ϊ��λ):\n");
            printf("    ��� = %.1fms��� = %.1fms��ƽ�� = %.1fms\n\n", min_rtt, max_rtt, avg_rtt / num_packets_received);
        }
        
        // �û���������
        else
		{
            // ����������ͱ���
            if (sendto(sockfd, input, strlen(input), 0, (struct sockaddr *) &server_addr, server_len) < 0)
			{
                perror("�������ݱ�ʧ��");
                closesocket(sockfd);
                WSACleanup();
				exit(EXIT_FAILURE);
            }

            // ���շ�������Ӧ
            if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, &server_len) < 0)
			{
                perror("�������ݱ�ʧ��");
                closesocket(sockfd);
                WSACleanup();
                exit(EXIT_FAILURE);
            }


            printf("���Է���������Ӧ��%s\n", buf);
        }
    }

    return 0;
}
