#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "Block.h"

void Game() ;
void ShowRecord() ;
void ShowBoard( GameBoard* gb ) ;
void ShowForm( GameBoard* gb ) ;
void ShowNextBlock( Block ) ;
void ShowHoldBlock( Block ) ;
void ShowScore( int total_score , int total_line , int combo ) ;
void EraseLine( GameBoard* , int* , int* , int* ) ;
bool GameOver( GameBoard* gb ) ;

#endif // GAME_H_INCLUDED
