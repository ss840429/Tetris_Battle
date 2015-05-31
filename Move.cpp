#include "Move.h"
#include "Block.h"
#include "Console.h"
#include <stdio.h>



void LoadToBuffer( GameBoard* buffer , Block b )
{
    buffer->Init(false) ;
    for( int i = 0 ; i < 4*4 ; i ++ )
    {
        if( Bricks[b.ReturnAtt().shape][i] )
        {
            buffer->Show( i/4 , (BoardSize_Y/2-2) + i%4 ).ReturnType() = Shape ;
            buffer->Show( i/4 , (BoardSize_Y/2-2) + i%4 ).ReturnAtt().color = b.ReturnAtt().color ;
        }
    }

    SetSpin_Node( buffer , b.ReturnAtt().shape ) ;

    int k = 1 ;
    do
    {
        if( !k )
            for( int i = 2 ; i >= 0 ; i -- )
                for( int j = 0 ; j < BoardSize_Y ; j ++ )
                    MoveBlock( buffer , i , j , i+1 , j ) ;

        k = 0 ;
        for( int i = 0 ; i < BoardSize_Y ; i ++ )
            k += (buffer->Show(3,i).ReturnType() == Shape) ;
    }
    while( !k ) ;
}
bool MoveBufferToBoard( GameBoard* buffer , GameBoard* gb )
{

    for( int col = 1 ; col < BoardSize_Y ; col ++ )
    {
        if( buffer->Show(3,col).ReturnType() == Shape && gb->Show(1,col).ReturnType() == Lock ) return false ;

        if( buffer->Show(3,col).ReturnType() == Shape )
        {
            gb->Show( 1 , col ) = buffer->Show( 3 , col ) ;
            buffer->Show( 3 , col ).Init() ;
        }
    }

    for( int row = 3 ; row > 0 ; row -- )
    {
        for( int col = 1 ; col < BoardSize_Y ; col ++ )
        {
            MoveBlock( buffer , row-1 , col , row , col ) ;
        }
    }
    return true ;
}

void MoveBlock( GameBoard* gb , int x , int y , int des_x , int des_y )
{
    if( !( gb->IsValid( x , y ) && gb->IsValid( des_x , des_y ) ) ) return ;

    gb->Show( des_x , des_y ) = gb->Show( x , y ) ;
    gb->Show( x , y ).Init() ;
}
int MoveDown( GameBoard* gb , GameBoard* buffer )
{
    if( Can_Move( gb , buffer , 1 , 0 ) )
    {
        for( int i = BoardSize_X - 2 ; i > 0 ; i -- )
            for( int j = BoardSize_Y - 2  ; j > 0 ; j -- )
                if( gb->Show(i,j).ReturnType() == Shape )
                    MoveBlock( gb , i , j , i + 1 , j ) ;

        MoveBufferToBoard( buffer , gb ) ;
        return 0 ;
    }
    else
    {
        for( int i = 1 ; i < BoardSize_X-1 ; i ++ )
            for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
                if( gb->Show(i,j).ReturnType() == Shape )
                    gb->Show(i,j).ReturnType() = Lock ;
        return 1 ;
    }

}
void MoveRight( GameBoard* gb , GameBoard* buffer )
{
    if( Can_Move( gb , buffer , 0 , 1 ) )
    {
        for( int i = 1 ; i < BoardSize_X - 1 ; i ++ )
            for( int j = BoardSize_Y - 3  ; j > 0 ; j -- )
                if( gb->Show(i,j).ReturnType() == Shape )
                    MoveBlock( gb , i , j , i , j+1 ) ;

        for( int i = 0 ; i < 4 ; i ++ )
            for( int j = BoardSize_Y - 3  ; j > 0 ; j -- )
                if( buffer->Show(i,j).ReturnType() == Shape )
                    MoveBlock( buffer , i , j , i , j+1 ) ;
    }
}
void MoveLeft( GameBoard* gb , GameBoard* buffer )
{
    if( Can_Move( gb , buffer , 0 , -1 ) )
    {
        for( int i = 1 ; i < BoardSize_X - 1 ; i ++ )
            for( int j = 1 ; j < BoardSize_Y - 1 ; j ++ )
                if( gb->Show(i,j).ReturnType() == Shape )
                    MoveBlock( gb , i , j , i , j-1 ) ;

        for( int i = 0 ; i < 4 ; i ++ )
            for( int j = 1 ; j < BoardSize_Y - 1 ; j ++ )
                if( buffer->Show(i,j).ReturnType() == Shape )
                    MoveBlock( buffer , i , j , i , j-1 ) ;
    }
}
void AllDown( GameBoard* gb , GameBoard* buffer )
{
    while( !MoveDown( gb , buffer ) ) ;
}
void Rotate( GameBoard* gb , GameBoard* buffer )
{
    GameBoard board ;
    board.Create( BoardSize_X-2 + 4 , BoardSize_Y - 2 ) , board.Init(false) ;

    for( int i = 0 ; i < 4 ; i ++ )         /* Shadow */
    {
        for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
        {
            board.Show(i,j-1) = buffer->Show( i , j ) ;
        }
    }
    for( int i = 1 ; i < BoardSize_X-1 ; i ++ )
    {
        for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
        {
            board.Show(4+(i-1),j-1) = gb->Show( i , j ) ;
        }
    }


    struct Mid
    {
        int x = -1 , y = -1 ;
    } mid ;

    GameBoard temp ;
    temp.Create(5,5) ;
    temp.Init(false) ;

    for( int i = 0 ; i < BoardSize_X+2 ; i ++  )  /* Search */
    {
        for( int j = 0 ; j < BoardSize_Y-1 ; j ++ )
        {
            if( board.Show(i,j).SetNode() && board.Show(i,j).ReturnType() == Shape )
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
            if( board.IsValid(mid.x-2+i,mid.y-2+j) && board.Show(mid.x-2+i,mid.y-2+j).ReturnType() == Shape )
            {
                temp.Show(j,4-i) = board.Show(mid.x-2+i,mid.y-2+j) ;
            }
        }
    }

    bool done = false ;

    if( ( done = Try_Rotate( &board , &temp , mid.x , mid.y ) ) ) ;
    else if( mid.y < 3 && ( done = Try_Rotate( &board , &temp , mid.x , mid.y+1 ) ) ) ;
    else if( mid.y < 3 && ( done = Try_Rotate( &board , &temp , mid.x , mid.y+2 ) ) ) ;
    else if( mid.y > BoardSize_Y-2 -3 && ( done = Try_Rotate( &board , &temp , mid.x , mid.y-1 ) ) ) ;
    else if( mid.y > BoardSize_Y-2 -3 && ( done = Try_Rotate( &board , &temp , mid.x , mid.y-2 ) ) ) ;
    else if( ( done = Try_Rotate( &board , &temp , mid.x+1 , mid.y-1 ) ) ) ;


    if( !done )
    {
        temp.Destroy() ;
        board.Destroy() ;
        return ;
    }

    for( int i = 0 ; i < 4 ; i ++ )         /* Shadow */
    {
        for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
        {
            buffer->Show( i , j ) = board.Show(i,j-1) ;
        }
    }
    for( int i = 1 ; i < BoardSize_X-1 ; i ++ )
    {
        for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
        {
            gb->Show( i , j ) = board.Show(4+(i-1),j-1) ;
        }
    }

    temp.Destroy() ;
    board.Destroy() ;
}

void Showpect(  GameBoard* gb , GameBoard* buffer )
{

    for( int i = 0 ; i < BoardSize_X ; i ++ )
        for( int j = 0 ; j < BoardSize_Y ; j ++ )
            if( gb->Show(i,j).ReturnType() == Empty )
                gb->Show(i,j).Init() ;

    GameBoard board ;
    board.Create( BoardSize_X-2 + 4 , BoardSize_Y - 2 ) , board.Init(false) ;

    for( int i = 0 ; i < 4 ; i ++ )         /* Shadow */
    {
        for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
        {
            board.Show(i,j-1) = buffer->Show( i , j ) ;
        }
    }
    for( int i = 1 ; i < BoardSize_X-1 ; i ++ )
    {
        for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
        {
            board.Show(4+(i-1),j-1) = gb->Show( i , j ) ;
        }
    }


    struct Mid
    {
        int x = -1 , y = -1 ;
    } mid , pmid ;

    GameBoard temp ;
    temp.Create(5,5) ;
    temp.Init(false) ;

    for( int i = 0 ; i < BoardSize_X+2 ; i ++  )  /* Search */
    {
        for( int j = 0 ; j < BoardSize_Y-1 ; j ++ )
        {
            if( board.Show(i,j).SetNode() && board.Show(i,j).ReturnType() == Shape )
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
            if( board.IsValid(mid.x-2+i,mid.y-2+j) && board.Show(mid.x-2+i,mid.y-2+j).ReturnType() == Shape )
            {
                temp.Show(i,j).ReturnType() = Shape ;
            }
        }
    }

    int i = 0 ;
    while( Check( &board , &temp , mid.x+i , mid.y ) ) i ++ ;

    pmid.x = mid.x+i-1 , pmid.y = mid.y ;

    for( int i = 0 ; i < 5 ; i ++ )    // Paste
    {
        for( int j = 0 ; j < 5 ; j ++ )
        {
            if( temp.Show(i,j).ReturnType() == Shape  )
            {
                board.Show( pmid.x-2+i , pmid.y-2+j ).ReturnType() = Empty ;
            }
        }
    }


    for( int i = 1 ; i < BoardSize_X-1 ; i ++ )
    {
        for( int j = 1 ; j < BoardSize_Y-1 ; j ++ )
        {
            if( board.Show(4+(i-1),j-1).ReturnType() == Empty && gb->Show(i,j).ReturnType() == None )
                gb->Show( i , j ) = board.Show(4+(i-1),j-1) ;
        }
    }

    temp.Destroy() ;
    board.Destroy() ;

}
void SetSpin_Node( GameBoard* buffer , int shape )
{
    char node[9][2] = { {1,1} , {1,2} , {1,1} , {1,2} , {2,3} , {2,0} , {2,1} , {1,1} , {1,1}} ;

    buffer->Show( node[shape][0] , node[shape][1]+BoardSize_Y/2-2 ).SetNode() = true ;
}

bool Can_Move( GameBoard* gb , GameBoard* buffer , int x , int y )
{
    bool Can = true ;

    for( int i = 0 ; i < BoardSize_X  ; i ++ )
        for( int j = 0 ; j < BoardSize_Y   ; j ++ )
            if( (gb->Show(i,j).ReturnType() == Shape &&
                    ( gb->Show(i+x,j+y).ReturnType() == Lock || gb->Show(i+x,j+y).ReturnType() == Wall )) )
            {
                Can = false ;
            }
    if( y == 0 ) return Can ;

    for( int i = 0 ; i < 4 ; i ++ )
        if( buffer->Show(i,1).ReturnType() == Shape && y == -1 )
            Can = false ;
        else if(buffer->Show(i,BoardSize_Y-2).ReturnType() == Shape && y == 1 )
            Can = false ;

    return Can ;
}
bool Check( GameBoard* board , GameBoard* temp , int midx  , int midy )
{
    if( !board->IsValid(midx,midy) ) return false ;

    for( int i = 0 ; i < 5 ; i ++ )     // Check
    {
        for( int j = 0 ; j < 5 ; j ++ )
        {
            if( temp->Show(i,j).ReturnType() == Shape &&( !board->IsValid( midx-2+i , midy-2+j ) || board->Show( midx-2+i , midy-2+j ).ReturnType() == Lock ) )
                return false ;
        }
    }
    return true ;
}
bool Try_Rotate( GameBoard* board , GameBoard* temp , int midx  , int midy )
{
    if( !Check( board , temp , midx , midy ) ) return false ;

    for( int i = 0 ; i < BoardSize_X+2 ; i ++  )    // Clear old
    {
        for( int j = 0 ; j < BoardSize_Y-1 ; j ++ )
        {
            if( board->Show(i,j).ReturnType() == Shape )
            {
                board->Show(i,j).Init() ;
            }
        }
    }


    for( int i = 0 ; i < 5 ; i ++ )    // Paste
    {
        for( int j = 0 ; j < 5 ; j ++ )
        {
            if( temp->Show(i,j).ReturnType() == Shape  )
            {
                board->Show( midx-2+i , midy-2+j ) = temp->Show(i,j) ;
            }
        }
    }

    return true ;
}

