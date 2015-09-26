#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <algorithm>
#include "Console.h"
#include "File.h"
#include "Block.h"
#include "Color.h"
#include "Move.h"
#include "Game.h"
#include "Location.h"
#include "Network.h"
#include "Music.h"

void ShowBoard( GameBoard& gb ) ;
void ShowEBoard( GameBoard& gb ) ;
void ShowForm() ;
void ShowNextBlock( Block ) ;
void ShowHoldBlock( Block ) ;
void ShowScore( int total_score , int total_line , int combo ) ;
void EraseLine( GameBoard& , int& , int& , int& ) ;
bool GameOver( GameBoard& gb ) ;


constexpr int level_time = 1000 ;                           /* ms -> speed */
constexpr int max_level_time = 150 ;
constexpr int Lscore[5] = { 0 , 20 , 100 , 250 , 500 } ;     /* Value of erasing each line */
Mode mode = Single ;



void Game(Mode M)
{
    Change_Mode( M ) ; mode = M ;
    if( M == Multi && SC() == 0 ) return ;

    Clean(); StopMusic();
    GameBoard board , enemy_board ;
    board.Create( BoardSize_X , BoardSize_Y ) , board.Init() ;
    enemy_board.Create( BoardSize_X , BoardSize_Y ) , enemy_board.Init() ;

    Block Cur , Next , Hold ;
    Cur.Init() , Next.Init() , Hold.Init() ;
    Cur.GainBlock() , Next.GainBlock() ;
    LoadToBuffer( board , Cur ) ;

    int Score = 0 , Line = 0 , Combo = 0 ;
    int tcounter = 0 , netcounter = 0 ;
    bool next , Can_Change = true ;

    ShowForm( ) ;
    PlayMusic() ;
    if( M == Multi ) Send(board) , Receive(enemy_board) ;

    do
    {
        
        ShowNextBlock( Next ) ;
        ShowHoldBlock( Hold ) ;
        Showpect( board ) ;
        ShowScore( Score , Line , Combo ) ;
        ShowBoard( board ) ;

        tcounter = clock() ;
        next = false ;

        while( !next )
        {
            netcounter ++ ;

            if( M == Multi && !(netcounter%100) )
            {
                netcounter = 0 ;
                Send(board) ;
                if( Receive( enemy_board ) )
                {
                    ShowEBoard( enemy_board ) ;
                }
            }

            int interact = KeyPressDetect() ;   /* User input */


            if( interact == Pause )
            {
                POINT old;
                GetCursorPos(&old);
                gotoxy( Pause_X , Pause_Y ) ;
                Drawtext( " ---- || PAUSE ---- " , Red ) ;
                while( !KeyPressDetect() ) ;
                gotoxy(old.x,old.y) ;
            }
            if( interact == Down || clock() - tcounter >= std::max( level_time - Line*4 , max_level_time )  ) /* down */
            {
                tcounter = clock() ;   /* reset time for next */
                next = MoveDown( board ) ;
                if( next ) delay(100) ;
                ShowBoard( board ) ;
                if( M == Multi ) ShowEBoard( enemy_board ) ;
            }
            if( interact == Space )
            {
                AllDown( board ) ;
                tcounter = clock() ;
                next = true ;
            }
            if( interact == Left ) MoveLeft( board ) ;

            if( interact == Right ) MoveRight( board ) ;

            if( interact == Up  && Cur.ReturnAtt().shape > 0 && Cur.ReturnAtt().shape < 7 )  Rotate( board ) ;

            if( interact == C && Can_Change )     /* C - Hold */
            {
                if( Hold.ReturnType() == None )  /* Fisrt Hold */
                {
                    Hold = Cur ;
                    next = true ;
                }
                else    std::swap( Hold , Cur ) ;    /* Swap Hold with Current */

                RmShape(board) ;
                Can_Change = false ;
                break ;
            }

            Showpect( board ) ;
            if( interact != 0 ) ShowBoard( board ) ;   // Refrash
        }

        if( next )
        {
            EraseLine( board , Score , Line , Combo ) ;
            Cur = Next , Next.GainBlock() ;
            Can_Change = true ;
            LoadToBuffer( board , Cur ) ;
        }
    }
    while( !GameOver(board) ) ;

    StopMusic() ;
    Close_Ws() ;
    board.Destroy(), enemy_board.Destroy() ;
    Clean();
    if( mode == Single ) FileSave(Score) ;
}

void ShowBoard( GameBoard& gb )
{
    gb.Print( Form_X+2 ,Form_Y+1 );
}
void ShowEBoard( GameBoard& gb )
{
    gb.Print( Form_X2+2 ,Form_Y2+1 );
}
void ShowForm()
{
    gotoxy( Form_X+6 , Form_Y-1 ) ;
    Drawtext( " Y  O  U  R  " , Yellow ) ;
    if( mode == Multi )
    {
        gotoxy( Form_X2+7 , Form_Y2-1 ) ;
        Drawtext( " R I V A L" , Yellow ) ;
    }
    for( int i = 0 ; i < BoardSize_X -2 ; i ++ ) /* Main Form */
    {
        gotoxy( Form_X , Form_Y + i ) ;
        for( int j = 0 ; j < BoardSize_Y + 2 ; j ++ )
            if( i == 0 || i == BoardSize_X-3 || j == 0 || j == BoardSize_Y+1 )
                DrawBlock(White) ;
            else
                printf( "  " ) ;

        if( mode == Multi )
        {
            gotoxy( Form_X2 , Form_Y2 + i ) ;
            for( int j = 0 ; j < BoardSize_Y + 2 ; j ++ )
                if( i == 0 || i == BoardSize_X-3 || j == 0 || j == BoardSize_Y+1 )
                    DrawBlock(White) ;
                else
                    printf( "  " ) ;
        }
    }
    for( int i = 0 ; i < 7 ; i ++ )
    {
        gotoxy( Next_X , Next_Y+i ) ;    /* Next Form */
        if( !i )
        {
            Drawtext( " N  E  X  T  " , Yellow ) ;
        }
        else if( i == 1 || i == 6 )
        {
            for( int j = 0 ; j < 6 ; j ++ )
                DrawBlock( White ) ;
        }
        else
        {
            DrawBlock( White ) ;
            printf( "        " ) ;
            DrawBlock( White ) ;
        }

        gotoxy( Hold_X , Hold_Y+i ) ;    /* Hold Form */
        if( !i )
        {
            Drawtext( " H  O  L  D  " , Yellow ) ;
        }
        else if( i == 1 || i == 6 )
        {
            for( int j = 0 ; j < 6 ; j ++ )
                DrawBlock( White ) ;
        }
        else
        {
            DrawBlock( White ) ;
            printf( "        " ) ;
            DrawBlock( White ) ;
        }
    }
}
void ShowNextBlock( Block next )
{
    for( int i = 0 ; i < 4 ; i ++ )
    {
        gotoxy( Next_X+2 , Next_Y+2+i ) ;
        for( int j = 0 ; j < 4 ; j ++ )
            if( Bricks[next.ReturnAtt().shape][i*4+j] )
                next.Print() ;
            else
                printf( "  " ) ;
    }
}
void ShowHoldBlock( Block hold )
{
    if( hold.ReturnType() == Shape )
    {
        for( int i = 0 ; i < 4 ; i ++ )
        {
            gotoxy( Hold_X+2 , Hold_Y+2+i ) ;
            for( int j = 0 ; j < 4 ; j ++ )
                if( Bricks[hold.ReturnAtt().shape][i*4+j] )
                    hold.Print() ;
                else
                    printf( "  " ) ;
        }
    }
}
void ShowScore( int total_score , int total_line , int combo )
{
    gotoxy( Score_X , Score_Y ) ;
    printf( "Score : %d " , total_score ) ;
    gotoxy( Score_X , Score_Y+2 ) ;
    printf( "Cleared : %d " , total_line ) ;
    gotoxy( Score_X , Score_Y+4 ) ;
    printf( "Combo : %d " , combo ) ;

}
void EraseLine( GameBoard& gb , int& score , int& line , int& combo )
{
    int add = 0 , ers [4] ;
    int i , j ;   /* For loop */

    for( i = BoardSize_X-1 ; i > 3 ; i -- )
    {
        int check = 0 ;
        for( j = 0 ; j < BoardSize_Y ; j ++ )
        {
            if( gb.Show(i,j).ReturnType() == Lock ) check ++ ;
        }

        if( check == BoardSize_Y )  ers[add++] = i + add ;
    }
    for( int i = 0 ; i < add ; i ++ )
    {
        for( int j = ers[i]-1 ; j > 0 ; j -- )
        {
            for( int k = 0 ; k < BoardSize_Y  ; k ++ )
                MoveBlock( gb , j , k , j+1 , k ) ;
        }
    }
    score += Lscore[add]*(1+combo) ;
    line += add ;
    if( add == 0 ) combo = 0 ;
    else combo += 1 ;
}
bool GameOver( GameBoard& board )
{
    for( int i = 0 ; i < BoardSize_Y ; i ++ )
        if( board.Show(3,i).ReturnType() == Lock )
            return true ;

    return false ;
}
