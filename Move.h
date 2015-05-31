#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "Block.h"

void LoadToBuffer( GameBoard* , Block ) ;
bool MoveBufferToBoard( GameBoard* , GameBoard* ) ;

void MoveBlock( GameBoard* gb , int x , int y , int desx , int dex_y ) ;
int  MoveDown( GameBoard* gb , GameBoard* buffer ) ;
void MoveRight( GameBoard* gb , GameBoard* buffer ) ;
void MoveLeft( GameBoard* gb , GameBoard* buffer ) ;
void AllDown( GameBoard* gb , GameBoard* buffer ) ;
void Rotate( GameBoard* , GameBoard* ) ;
void Showpect(  GameBoard* , GameBoard* ) ;

void SetSpin_Node( GameBoard*  , int  ) ;

bool Can_Move( GameBoard* , GameBoard* , int  , int  ) ;
bool Check( GameBoard* board , GameBoard* temp , int midx  , int midy ) ;
bool Try_Rotate(  GameBoard* , GameBoard* , int  , int ) ;

#endif // MOVE_H_INCLUDED
