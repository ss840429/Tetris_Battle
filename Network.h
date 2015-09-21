#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "Block.h"

struct Packet{

    Block board[BoardSize_X][BoardSize_Y] ;

} ;

int SC() ;
int Init_Ws() ;
void Close_Ws() ;
int Receive( GameBoard& gb ) ;
void Send( GameBoard& gb ) ;
int Host() ;
int Client() ;


#endif // NETWORK_H_INCLUDED
