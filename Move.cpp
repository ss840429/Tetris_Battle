#include "Move.h"
#include "Block.h"
#include "Console.h"
#include <stdio.h>

bool Can_Move( GameBoard& , int  , int  ) ;
bool Try_Rotate(  GameBoard& , GameBoard& , int  , int ) ;
void SetSpin_Node( GameBoard& gb , int x ) ;


void LoadToBuffer( GameBoard& buffer , Block b )
{
    for( int i = 0 ; i < 4*4 ; i ++ )
    {
        if( Bricks[b.ReturnAtt().shape][i] )
        {
            buffer.Show( i/4 , (BoardSize_Y/2-2) + i%4 ).ReturnType() = Shape ;
            buffer.Show( i/4 , (BoardSize_Y/2-2) + i%4 ).ReturnAtt().color = b.ReturnAtt().color ;
        }
        else
            buffer.Show( i/4 , (BoardSize_Y/2-2) + i%4 ).Init() ;
    }

    SetSpin_Node( buffer , b.ReturnAtt().shape ) ;

    while( !MoveDown( buffer ) )
    {
        bool k = false ;
        for( int i = 0 ; i < BoardSize_Y ; i ++ )
           if( buffer.Show(4,i).ReturnType() == Shape )
                k = true ;

        if( k ) break ;
    }
}
void MoveBlock( GameBoard& gb , int x , int y , int des_x , int des_y )
{
    if(  !( gb.IsValid( x , y ) && gb.IsValid( des_x , des_y ) )  ) return ;

    gb.Show( des_x , des_y ) = gb.Show( x , y ) ;
    gb.Show( x , y ).Init() ;
}
int MoveDown( GameBoard& gb )
{
    if( Can_Move( gb , 1 , 0 ) )
    {
        for( int i = BoardSize_X - 2 ; i >= 0 ; i -- )
            for( int j = 0 ; j < BoardSize_Y ; j ++  )
                if( gb.Show(i,j).ReturnType() == Shape )
                    MoveBlock( gb , i , j , i + 1 , j ) ;

        return 0 ;
    }
    else        /* Lock Block */
    {
        for( int i = 0 ; i < BoardSize_X ; i ++ )
            for( int j = 0 ; j < BoardSize_Y ; j ++ )
                if( gb.Show(i,j).ReturnType() == Shape )
                    gb.Show(i,j).ReturnType() = Lock ;
        return 1 ;
    }

}
void MoveRight( GameBoard& gb )
{
    if( Can_Move( gb , 0 , 1 ) )
    {
        for( int i = 0 ; i < BoardSize_X ; i ++ )
            for( int j = BoardSize_Y - 2  ; j >= 0 ; j -- )
                if( gb.Show(i,j).ReturnType() == Shape )
                    MoveBlock( gb , i , j , i , j+1 ) ;
    }
}
void MoveLeft( GameBoard& gb )
{
    if( Can_Move( gb , 0 , -1 ) )
    {
        for( int i = 0 ; i < BoardSize_X ; i ++ )
            for( int j = 1 ; j < BoardSize_Y ; j ++ )
                if( gb.Show(i,j).ReturnType() == Shape )
                    MoveBlock( gb , i , j , i , j-1 ) ;
    }
}
void AllDown( GameBoard& gb )
{
    while( !MoveDown( gb ) ) ;
}
void Rotate( GameBoard& gb )
{
    struct Mid
    {
        int x = -1 , y = -1 ;
    } mid ;

    GameBoard temp ;
    temp.Create(5,5) ;
    temp.Init() ;

    for( int i = 0 ; i < BoardSize_X+4 ; i ++  )  /* Search */
    {
        for( int j = 0 ; j < BoardSize_Y ; j ++ )
        {
            if( gb.Show(i,j).SetNode() && gb.Show(i,j).ReturnType() == Shape )
            {
                mid.x = i , mid.y = j ;
                break ;
            }
        }
        if( mid.x != -1 ) break ;
    }

    for( int i = 0 ; i < 5 ; i ++ )         /* Produce Graph */
    {
        for( int j = 0 ; j < 5 ; j ++ )
        {
            if( gb.Show(mid.x-2+i,mid.y-2+j).ReturnType() == Shape )
            {
                temp.Show(j,4-i) = gb.Show(mid.x-2+i,mid.y-2+j) ;
            }
        }
    }

    Try_Rotate( gb , temp , mid.x , mid.y ) ;
    temp.Destroy() ;
}

void Showpect( GameBoard& gb )
{
    for( int i = 0 ; i < BoardSize_X ; i ++ )
        for( int j = 0 ; j < BoardSize_Y ; j ++ )
            if( gb.Show(i,j).ReturnType() == Empty )
                gb.Show(i,j).Init() ;

    int maxdis = 100 , t = 0 ;
    int i , j ;

    for( i = 0 ; i < BoardSize_Y ; i ++  )  /* Search */
    {
        t = 0 ;
        for( j = 0 ; j < BoardSize_X ; j ++ )
        {
            if( gb.Show(j,i).ReturnType() == Shape ) t = 0 ;
            else if( gb.Show(j,i).ReturnType() == Lock )
            {
                if( maxdis > t ) maxdis = t ;
                break ;
            }
            else t ++ ;
        }
        if( j == BoardSize_X  && maxdis > t ) maxdis = t ;

    }

    for( int i = 0 ; i < BoardSize_X+4 ; i ++  )  /* Paste */
        for( int j = 0 ; j < BoardSize_Y ; j ++ )
            if( gb.Show(i,j).ReturnType() == Shape && gb.Show(i+maxdis,j).ReturnType() == None  )
                gb.Show(i+maxdis,j).ReturnType() = Empty ;

}
void SetSpin_Node( GameBoard& buffer , int shape )
{
    char node[9][2] = { {1,1} , {1,2} , {1,1} , {1,2} , {2,3} , {2,0} , {2,1} , {1,1} , {1,1}} ;

    buffer.Show( node[shape][0] , node[shape][1]+BoardSize_Y/2-2 ).SetNode() = true ;
}

bool Can_Move( GameBoard& gb , int x , int y )
{
    bool Can = true ;

    for( int i = 0 ; i < BoardSize_X  ; i ++ )
        for( int j = 0 ; j < BoardSize_Y   ; j ++ )
            if( gb.Show(i,j).ReturnType() == Shape &&
                    ( gb.Show(i+x,j+y).ReturnType() == Lock || !gb.IsValid(i+x,j+y) ) )
            {
                Can = false ;
            }

    return Can ;
}
bool Check( GameBoard& board , GameBoard& temp , int midx  , int midy )
{
    for( int i = 0 ; i < 5 ; i ++ )
        for( int j = 0 ; j < 5 ; j ++ )
        {
            if( !board.IsValid(midx-2+i,midy-2+j) && temp.Show(i,j).ReturnType() == Shape )
                return false ;
            if( board.Show(midx-2+i,midy-2+j).ReturnType() == Lock && temp.Show(i,j).ReturnType() == Shape )
                return false ;
        }
    return true ;
}
bool Try_Rotate( GameBoard& board , GameBoard& temp , int midx  , int midy )
{
    if( Check( board , temp , midx , midy ) ) ;
    else if( midy+3 > BoardSize_Y && Check( board , temp , midx , midy-1 ) ) midy -- ;
    else if( midy+3 > BoardSize_Y && Check( board , temp , midx , midy-2 ) ) midy -= 2 ;
    else if( midy-3 < 0 && Check( board , temp , midx , midy+1 ) ) midy ++ ;
    else if( midy-3 < 0 && Check( board , temp , midx , midy+2 ) ) midy += 2 ;
    else if( Check( board , temp , midx+1 , midy-1 ) ) midy -- ;
    else return false ;

    RmShape(board) ;

    for( int i = 0 ; i < 5 ; i ++ )
        for( int j = 0 ; j < 5 ; j ++ )
        {
            if( !board.IsValid(midx-2+i,midy-2+j)) continue ;

            if( temp.Show(i,j).ReturnType() == Shape )
                board.Show(midx-2+i,midy-2+j) = temp.Show(i,j) ;
            else if( board.Show(midx-2+i,midy-2+j).ReturnType() != Lock )
                board.Show(midx-2+i,midy-2+j).Init() ;
        }
    return true ;
}
void RmShape( GameBoard& gb )
{
    for( int i = 0 ; i < BoardSize_X ; i ++ )
        for( int j = 0 ; j < BoardSize_Y ; j ++ )
            if( gb.Show(i,j).ReturnType() == Shape )
                gb.Show(i,j).Init() ;
}
