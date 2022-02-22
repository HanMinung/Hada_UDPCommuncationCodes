#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 30
char* UDP_SERVER_IP = "192.168.1.12";
unsigned int UDP_PORT = 2001;

void ErrorHandling(char* message);

int main(void)
{
    WSADATA wsaData;
    SOCKET sock;
    char message[BUF_SIZE] = "Hi! python";
    int strLen, adrSize;

    SOCKADDR_IN servAdr, clntAdr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    sock = socket(PF_INET, SOCK_DGRAM, 0);              // STEP 1 : UDP socket�� ����

    /*
        int socket (int domain, int type, int protocol)
        return value : ���� �� ������ �ƴ� �� / ���� �� -1
    */

    if (sock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(UDP_SERVER_IP);
    servAdr.sin_port = htons(UDP_PORT);

    int send_data[4] = { 1,2,3,4 };
    int recv_data[4] = { 0, };

    /*
        int sendto(int sock, const void *msg, int len, unsigned flags, const struct sockadder *addr, int addrlen)
            sock : �����͸� ������ �� ����� ������ ���� ��ũ����
            msg : �����ϰ��� �ϴ� �����͸� ������ ���� ���۸� ����Ű�� ������
            len : size of (pointer -- n byte)
            flags : generally zero setting
            addr : �����ϰ��� �ϴ� ���� �ּ� ������ �ʱ�ȭ�� ������ ���ڷ� �ѱ��
            addrlen : addr pointer�� ����Ű�� �ִ� ����ü ������ ũ��

        ���� �� ���۵� byte �� , ���� �� -1 return 
    */

    while (1)
    {
        sendto(sock, send_data, 16, 0, (struct sockaddr*)&servAdr, sizeof(servAdr));    //������ (16 byte)
        adrSize = sizeof(clntAdr);
        recvfrom(sock, recv_data, 16, 0, (struct sockaddr*)&clntAdr, &adrSize);         //�޴´� (16 byte)

        printf("recv_data : %d %d %d %d\n", recv_data[0], recv_data[1], recv_data[2], recv_data[3]);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}