#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "Network.h"
#include "Block.h"
#include "Console.h"
#include "Color.h"

#define ConnectPort 80

char* GetLocalIP() ;
void DtoP( GameBoard gb , Packet& p) ;
void PtoD( Packet p , GameBoard& gb ) ;
void Verify( Packet p ) ;

int Listen_id = 0 , Client_id = 0 ;
struct sockaddr_in ServerInfo , ClientInfo ;
char IP[30] ;

int Init_Ws()
{
    WSADATA wsadata;
    WORD version = MAKEWORD(2,2) ;          /* Winsock 2.2 */
    return WSAStartup( version , &wsadata ) ;
}

void Close_Ws()
{
    closesocket(Listen_id) ;
    closesocket(Client_id) ;
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

int SC()
{
    LableA :
    CleanSel() ;
    if( Init_Ws() )     // Init
    {
        CleanSel() ;
        Draw( 29 , 15 , "Initialization failed" , 0 ) ;
        Draw( 25 , 18 , "Please check PC's environment" , 0 ) ;
        Draw( 38 , 21 , "[OK]" , true ) ;
        while( KeyPressDetect() != Enter ) ;
        CleanSel() ;
        return 0 ;
    }

    int state = 1 , key , prekey = -1 ;
    do
    {
        key = KeyPressDetect() ;
        if( key == Up && state != 1 ) state -- ;
        if( key == Down && state != 3 ) state ++ ;
        if( key == prekey ) continue ;
        prekey = key ;

        Draw( 34 , 15 , "(1) Host" , state == 1 ) ;
        Draw( 33 , 18 , "(2) Client" , state == 2 ) ;
        Draw( 30 , 21 , "(3) Back to Menu\n" , state == 3 ) ;

        printf("\n                                                            ") ;
    }
    while( key != Enter ) ;

    CleanSel() ;
    if( state == 1 )            // Host
    {
        Draw( 35 , 14 , "Yout IP :" , 0 ) ;
        Draw( 33 , 15 , GetLocalIP() , 1 ) ;
        Draw( 28 , 18 , "Waiting for connection ..." , 0 ) ;
        Draw( 31 , 21 , "Press ESC to abort " , 0 ) ;
        int r = Host() ;
        if( r == -1 ) goto LableA ;
    }
    else if( state == 2 )       // Client
    {
        Draw( 31 , 14 , "Input Server IP :" , 0 ) ;
        Draw( 31 , 16 , "=> " , 0 ) ;
        scanf( "%s" , IP ) ;
        int r = Client() ;

        if( r == -1 )
        {
            delay( 500 ) ;
            gotoxy(31,18);
            Drawtext(  "Connect failed.." , Red ) ;
            delay(3000) ;
            goto LableA ;
        }

    }
    else
    {
        Close_Ws() ;
        CleanSel() ;
        return 0 ;
    }

    CleanSel() ;
    state = 1 ;
    gotoxy(31,15) ;
    Drawtext( "C o n n e c t e d" , Red ) ;
    do
    {
        key = KeyPressDetect() ;
        if( key == Up && state != 1 ) state -- ;
        if( key == Down && state != 2 ) state ++ ;
        if( key == prekey ) continue ;
        prekey = key ;

        Draw( 35 , 18 , " Start !!" , state == 1 ) ;
        Draw( 35 , 21 , " Abort .." , state == 2 ) ;

        printf("\n                                                            ") ;
    }while( key != Enter ) ;

    char confirm[3] = "" , sconfirm[3] = "OK" ;

    if( state == 2 )
    {
        Close_Ws() ;
        CleanSel() ;
        return 0 ;
    }
    else if( state == 1 )
    {
        gotoxy(31,15) ;
        Drawtext( "Waiting for rival .." , Red ) ;
        while( strcmp( confirm , sconfirm ) )
        {
            send( Client_id , sconfirm , sizeof(sconfirm) ,0 ) ;
            recv( Client_id , confirm , sizeof(confirm) , 0 ) ;
        }
        CleanSel() ;
        return 1 ;
    }
}

int Host()
{
    Listen_id = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
    unsigned long val = 1; ;

    ServerInfo.sin_family = AF_INET ;
    ServerInfo.sin_port = htons(ConnectPort) ;
    ServerInfo.sin_addr.s_addr = INADDR_ANY ;

    bind( Listen_id , ( struct sockaddr* )&ServerInfo , sizeof(ServerInfo) ) ;
    listen( Listen_id , 1 ) ;

    ioctlsocket( Listen_id , FIONBIO, &val ) ; /* Non-Blocking mode */

    int Cinfolen = sizeof( ClientInfo ) ;
    while( ( Client_id = accept( Listen_id , ( struct sockaddr* )&ClientInfo , &Cinfolen ) ) )
    {
        if( Client_id != INVALID_SOCKET )
        {
            ioctlsocket( Client_id , FIONBIO, &val ) ; /* Non-Blocking mode */
            return 1 ;
        }
        if( KeyPressDetect() == ESC )
        {
            return -1 ;
        }
    }
    return -1 ;
}

int Client()
{
    Client_id = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
    unsigned long val = 1; ;

    ServerInfo.sin_family = AF_INET ;
    ServerInfo.sin_port = htons(ConnectPort) ;
    ServerInfo.sin_addr.s_addr = inet_addr(IP) ;

    int r = connect( Client_id , ( struct sockaddr* )&ServerInfo , sizeof(ServerInfo)) ;

    if( r == 0 )
    {
        ioctlsocket( Client_id , FIONBIO, &val ) ; /* Non-Blocking mode */
        return 1 ;
    }
    else return -1 ;
}

Packet SP , RP , BRP ;

int Receive( GameBoard& gb )
{
    int n = recv( Client_id , (char*)&RP , sizeof(RP) , 0 ) ;

    if( n <= 0  ) return 0 ;
    else
    {
        Verify(RP) ;
        PtoD( RP , gb ) ;
        RP = BRP ;
    }

    return 1 ;
}
void Send( GameBoard& gb )
{
    DtoP( gb , SP ) ;
    send( Client_id , (char*)&SP , sizeof(SP) , 0 ) ;
}

void DtoP( GameBoard gb , Packet& p )
{
    for( int i = 0 ; i < BoardSize_X ; i ++ )
        for( int j = 0 ; j < BoardSize_Y ; j ++ )
            p.board[i][j] = gb.Show(i,j) ;
}

void PtoD( Packet p , GameBoard& gb )
{
    for( int i = 0 ; i < BoardSize_X ; i ++ )
        for( int j = 0 ; j < BoardSize_Y ; j ++ )
                gb.Show(i,j) = p.board[i][j] ;

}
void Verify( Packet p )
{
     for( int i = 1 ; i < BoardSize_X-1 ; i ++ )
        for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
            if( p.board[i][j].ReturnAtt().color == White && p.board[i][j].ReturnType() == Shape )
                RP = BRP ;
}
