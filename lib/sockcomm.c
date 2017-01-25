#include<stdio.h>
#include<winsock2.h>

typedef unsigned char Byte;
#define  BUFF_SIZE   1024*4

void ErrorHandling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}

SOCKET sock_init(){
    WSADATA wsaData;
    SOCKET client_socket;

    // Load WinSocket 2.2 DLL
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
            ErrorHandling("WSAStartup(), error");
    }

    client_socket  = socket( PF_INET, SOCK_STREAM, 0);

    if( INVALID_SOCKET == client_socket)
    {
        printf( "socket create fail.\n");
        exit( 1);
    }

    return client_socket;
}

void sock_conn(SOCKET client_socket, char addr[16], int port){
    struct sockaddr_in server_addr;
    char buff[BUFF_SIZE+5];

    memset( &server_addr, 0, sizeof( server_addr));
    server_addr.sin_family     = AF_INET;
    server_addr.sin_port       = htons(port);
    server_addr.sin_addr.s_addr= inet_addr(addr);

    if( SOCKET_ERROR == connect( client_socket, (SOCKADDR*)&server_addr, sizeof( server_addr) ) )
    {
          ErrorHandling("Connect() error");
    }
}

SOCKET sock(char addr[16], int port){
    SOCKET client_socket;

    client_socket = sock_init();
    sock_conn(client_socket, addr, port);
    return client_socket;
}

#if 0 // main() test
void proc(SOCKET client_socket){
    char buff[BUFF_SIZE+5] = {0};
    Byte plain_text[16] = {0x8d, 0x14, 0x70, 0x62, 0x5f, 0x59, 0xeb, 0xac, 0xb0, 0xe5, 0x5b, 0x53, 0x4b, 0x3e, 0x46, 0x2b};
    Byte return_text[BUFF_SIZE+5] = {0};
    int i;

    send( client_socket, plain_text, strlen(plain_text)+1, 0);
    recv( client_socket, return_text, BUFF_SIZE, 0);
    for(i=0; return_text[i]!=0; i++)
    {
        printf( "0x%02x ", return_text[i]);
    }
    printf("\n");
}


int main(){
    int i;
    SOCKET client_socket = 0;

    client_socket = sock_init();
    sock_conn(client_socket, "127.0.0.1", 9001);
    proc(client_socket);
    closesocket(client_socket);

    return 0;
}
#endif
