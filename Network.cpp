#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib,"libws2_32.a")
#define ConnectPort 8888

void init_winsock()
{
    WSADATA wsadata;
    WSAStartup( MAKEWORD(2,2), &wsadata);
}

void close_winsock()
{
    WSACleanup() ;
}

char* GetLocalIP()
{
	char szHostName[128];
  	struct hostent *pHost;

   	gethostname( szHostName, 128 );
   	pHost = gethostbyname( szHostName );
   	return inet_ntoa( *(struct in_addr*)pHost->h_addr_list[0] ) ;
} 

void be_host()
{
    int Listenid = socket( PF_INET , SOCK_STREAM , 0 ) ;
    unsigned long val = 1; ;
    struct sockaddr_in LocalInfo , Clientinfo ;

    LocalInfo.sin_family = PF_INET ;
    LocalInfo.sin_port = htons(ConnectPort) ;
    LocalInfo.sin_addr.s_addr = INADDR_ANY ;

    bind( Listenid , ( struct sockaddr* )&LocalInfo , sizeof(LocalInfo) ) ;
    listen( Listenid , 1 ) ;

    int Clientid , Cinfolen = sizeof( Clientinfo ) ;

    Clientid = accept( Listenid , ( struct sockaddr* )&Clientinfo , &Cinfolen ) ;

    ioctlsocket( Listenid , FIONBIO, &val ) ; /* Non-Blocking mode */
}

void be_client()
{

}

