#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "Block.h"

void LoadToBuffer( GameBoard& , Block ) ;

void MoveBlock( GameBoard& gb , int x , int y , int desx , int dex_y ) ;
int  MoveDown( GameBoard&   ) ;
void MoveRight( GameBoard&  ) ;
void MoveLeft( GameBoard& ) ;
void AllDown( GameBoard& ) ;
void Rotate( GameBoard& ) ;
void Showpect( GameBoard& gb ) ;
void RmShape( GameBoard& gb ) ;




#endif // MOVE_H_INCLUDED
