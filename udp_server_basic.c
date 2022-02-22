
 //UDP/IP ��ſ����� recvfrom(), sendto() �Լ��� �̿��� �ڷḦ �ۼ����Ѵ�.

 //   1. sendto() �Լ��� ������ �������� ������ �� �ְ� recvfrom() �Լ��� ���� �����Ϳ� �۽����� ������ ���� �� �ִ�.
 //   2. ���� ������ bind()�� ���� ���Ͽ� �ּҿ� ��Ʈ�� ���������� �Ѵ�. 
 //   3. sendto()�� ���� ȣ���ϴ� ���� bind�� ������ �ʾƵ� �ȴ�
 //      -> �Լ��� ȣ���� �� ���ڷ� �־����� ���� �ּ������� �����Ͽ� �ü���� �ڵ����� bind ���ֱ� ����



#define  WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#define BUF_SIZE 300
unsigned int UDP_PORT = 2001;
void ErrorHandling(char* message);

int main()
{
    WSADATA wsaData;
    SOCKET servSock;
    char message[BUF_SIZE] = "I'm C!!\n";
    int strLen;
    int clntAdrSz;

    SOCKADDR_IN servAdr, clntAdr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)     
        ErrorHandling("WSAStartup() error!");

    servSock = socket(PF_INET, SOCK_DGRAM, 0);         // UDP ���� ������ ���ؼ� socket �Լ��� �ι�° ���ڷ� SOCK_DGRAM�� ����
    if (servSock == INVALID_SOCKET)
        ErrorHandling("UDP socket creation error!");

    /*
        socket�� �����ϴ� �Լ� , socket���� IP�ּҸ� �Ҵ��ؾ� �Ѵ�. 
        ���� �� : file descripter & ���� �� -1 return

        scoket�� �ּҸ� �Ҵ��ϴ� �Լ� : bind
        ���� �� 0 , ���� �� -1 return 

        UDP ��� : TCP ��� ���� ���α׷����� ȣ���ϴ� listen �Լ��� accept �Լ�, connect �Լ��� ȣ���� ���ʿ��ϴ�. ������ ���� ���� & �ּ� �Ҵ�
    */

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(UDP_PORT);

    if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    clntAdrSz = sizeof(clntAdr);
    int recv_data[4] = { 0, };
    int send_data[4] = { 4,3,2,1 };
    while (1)
    {
        strLen = recvfrom(servSock, recv_data, 16, 0, (SOCKADDR*)&clntAdr, &clntAdrSz); //�Ҵ�� �ּҷ� ���޵Ǵ� ��� �����͸� ����, ���� �������� ���޴�󿡴� ������ ����
        printf("recv data : %d %d %d %d \n", recv_data[0], recv_data[1], recv_data[2], recv_data[3]);
        
        sendto(servSock, send_data, 16, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr)); //�Լ�ȣ���� ���ؼ� �����͸������� ���� �ּ������� �Բ� ��� �Ǵµ�, �ٷ� �� �ּ������� �̿��ؼ� ���ŵ� �����͸� ������ ������

    }

    closesocket(servSock);
    WSACleanup();
    return 0;
}



void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
