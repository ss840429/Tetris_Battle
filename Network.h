#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED


void init_winsock() ;
void close_winsock() ;
void receive() ;
void send() ;
void be_host() ;
void be_client() ;
char* GetLocalIP() ;

#endif // NETWORK_H_INCLUDED
