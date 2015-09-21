#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include "Console.h"
#include <stdio.h>

const int BoardSize_X = 24 , BoardSize_Y = 10 ;  /* Size , extra for buffer */
const int Chance_trap = 2 ; /* x of 100 */

const int Bricks[9][16] = {
                        {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},      // Square
                        {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},      // Line
                        {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},      // _|-
                        {0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0},      // -|_
                        {0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0},      // converse L
                        {0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},      // L
                        {0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},      // T
                        {1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1},      // Trap 1
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}      // Trap 2
                        ;

enum BlockType { None , Shape , Lock , Empty } ;

struct Attribute
{
        int shape ;
        int color ;
} ;

class Block
{
        public:
                Attribute& ReturnAtt()
                {
                        return att ;
                }
                bool& SetNode( )
                {
                        return IsSNode ;
                }
                BlockType& ReturnType()
                {
                        return type ;
                }
                void Print()
                {
                    if( type == Empty )
                        printf( "¡¼" ) ;
                    else if( type != None )
                        DrawBlock( att.color ) ;
                    else
                        printf( "  " ) ;
                }
                void Init() ;
                void GainBlock() ;
        private:
                Attribute att ;
                BlockType type ;
                bool IsSNode ;
} ;

class GameBoard
{

        public:
                void Create( int sizex , int sizey ) ;
                void Init( ) ;
                void Destroy() ;
                bool IsEmpty() ;
                void Print( int startx , int starty ) ;
                bool IsValid( int x , int y ) ;
                Block& Show( int row , int col )
                {
                    if( IsValid(row,col) )
                        return board[row][col] ;
                    else
                        return board[0][0];
                }
        private:
                Block** board ;
                int x , y ;
} ;

#endif // BLOCK_H_INCLUDED
