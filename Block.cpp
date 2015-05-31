#include "Console.h"
#include "Block.h"
#include <stdlib.h>
#include <stdio.h>
#include "Color.h"

void Block::GainBlock()
{
    att.color = random(0,5) ;
    type = Shape ;

    if( random(1,100) <= Chance_trap )
        att.shape = random(7,8) ;
    else
        att.shape = random(0,6) ;
}
void Block::Init()
{
    att.shape = -1 ;
    att.color =  White ;
    type = None ;
    IsSNode = 0 ;
}
void GameBoard::Create( int sizex , int sizey )
{
    x = 0 , y = 0 ;

    board = (Block**)malloc( sizeof(Block*) * sizex ) ;
    if( board == NULL ) return ;

    for( int row = 0 ; row < sizex ; row ++ )
    {
        board[row] = (Block*)malloc( sizeof(Block) * sizey ) ;
        if( board[row] == NULL )
        {
            for( int col = 0 ; col < row ; col ++ )
                free( board[col] ) ;
            free( board ) ;

            return ;
        }
    }

    x = sizex , y = sizey ;
}
void GameBoard::Init( bool SetWall )
{
    for( int row = 0 ; row < x ; row ++ )
    {
        for( int col = 0 ; col < y ; col ++ )
        {
            board[row][col].Init() ;

            if( SetWall && ( !row || !col || row == x-1 || col == y-1 ) )  // wall set
                board[row][col].ReturnType() = Wall ;
        }
    }
}
void GameBoard::Print( int startx , int starty )
{
     for( int i = 1 ; i < BoardSize_X -1 ; i ++ )
    {
        gotoxy( startx , starty-1 + i ) ;
        for( int j = 1 ; j < BoardSize_Y -1 ; j ++ )
                board[i][j].Print() ;
    }
}
void GameBoard::Destroy()
{
    for( int row = 0 ; row < x ; row ++ )
    {
        free( board[row] ) ;
    }
    free( board ) ;

    x = 0 ;
    y = 0 ;
}
bool GameBoard::IsEmpty()
{
    int k = 0 ;
    for( int row = 0 ; row < x ; row ++ )
        for( int col = 0 ; col < y ; col ++ )
            k += ( board[row][col].ReturnType() == Shape ) ;

    return !k ;
}
bool GameBoard::IsValid( int tx , int ty )
{
    return ( tx >= 0 && ty >= 0 && tx < x && ty < y ) ;
}
