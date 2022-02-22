
 //UDP/IP 통신에서는 recvfrom(), sendto() 함수를 이용해 자료를 송수신한다.

 //   1. sendto() 함수는 전송할 목적지를 저장할 수 있고 recvfrom() 함수는 수신 데이터와 송신자의 정보도 얻을 수 있다.
 //   2. 수신 측에서 bind()를 통해 소켓에 주소와 포트를 연결시켜줘야 한다. 
 //   3. sendto()를 먼저 호출하는 경우는 bind를 해주지 않아도 된다
 //      -> 함수를 호출할 때 인자로 주어지는 상대방 주소정보를 참조하여 운영체제가 자동으로 bind 해주기 때문



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

    servSock = socket(PF_INET, SOCK_DGRAM, 0);         // UDP 소켓 생성을 위해서 socket 함수의 두번째 인자로 SOCK_DGRAM을 전달
    if (servSock == INVALID_SOCKET)
        ErrorHandling("UDP socket creation error!");

    /*
        socket을 생성하는 함수 , socket에도 IP주소를 할당해야 한다. 
        성공 시 : file descripter & 실패 시 -1 return

        scoket에 주소를 할당하는 함수 : bind
        성공 시 0 , 실패 시 -1 return 

        UDP 통신 : TCP 기반 서버 프로그램에서 호출하던 listen 함수와 accept 함수, connect 함수의 호출은 불필요하다. 오로지 소켓 생성 & 주소 할당
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
        strLen = recvfrom(servSock, recv_data, 16, 0, (SOCKADDR*)&clntAdr, &clntAdrSz); //할당된 주소로 전달되는 모든 데이터를 수신, 물론 데이터의 전달대상에는 제한이 없다
        printf("recv data : %d %d %d %d \n", recv_data[0], recv_data[1], recv_data[2], recv_data[3]);
        
        sendto(servSock, send_data, 16, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr)); //함수호출을 통해서 데이터를전송한 이의 주소정보도 함께 얻게 되는데, 바로 이 주소정보를 이용해서 수신된 데이터를 역으로 재전송

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
