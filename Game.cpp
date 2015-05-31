#include <stdio.h>
#include <time.h>
#include "Console.h"
#include "File.h"
#include "Block.h"
#include "Color.h"
#include "Move.h"
#include "Game.h"
#include <windows.h>
#include <algorithm>


constexpr int level_time = 300 ;                           /* ms -> speed */
constexpr int Lscore[5] = { 0 , 10 , 50 , 150 , 500 } ;     /* Value of erasing each line */

void Game()
{
    Clean();
    GameBoard board , buffer ;
    board.Create( BoardSize_X , BoardSize_Y ) , board.Init( true ) ;
    buffer.Create( 4 , BoardSize_Y ) , buffer.Init( false ) ;

    Block Cur , Next , Hold ;
    Cur.Init() , Next.Init() , Hold.Init() ;
    Cur.GainBlock() , Next.GainBlock() ;

    int Score = 0 , Line = 0 , Combo = 0 ;
    int tcounter = 0 ;
    bool next , Can_Change = true ;

    ShowForm( &board ) ;

    do
    {
        LoadToBuffer( &buffer , Cur ) ;
        if( !MoveBufferToBoard( &buffer , & board) ) break ;

        ShowNextBlock( Next ) ;
        ShowHoldBlock( Hold ) ;
        Showpect( &board , &buffer ) ;
        ShowBoard( &board ) ;
        ShowScore( Score , Line , Combo ) ;

        tcounter = clock() ;
        next = false ;

        while( !next )
        {
            int interact = KeyPressDetect() ;   /* User input */

            if( interact == Pause )
            {
                POINT old;
                GetCursorPos(&old);
                gotoxy( 27 , 13 ) ;
                Drawtext( " ---- || PAUSE ---- " , Red ) ;
                while( !KeyPressDetect() ) ;
                gotoxy(old.x,old.y) ;
            }
            if( interact == Down || clock() - tcounter >= level_time - Line * 4 ) /* down */
            {
                tcounter = clock() ;   /* reset time for next */
                next = MoveDown( &board , &buffer) ;
                if( next ) delay(100) ;
                ShowBoard( &board ) ;
            }
            if( interact == Space )
            {
                AllDown( &board , &buffer ) ;
                tcounter = clock() ;
                next = true ;
            }
            if( interact == Left ) MoveLeft( &board , &buffer ) ;

            if( interact == Right ) MoveRight( &board , &buffer ) ;

            if( interact == Up  && Cur.ReturnAtt().shape > 0 && Cur.ReturnAtt().shape < 7 )
            {
                Rotate( &board , &buffer) ;
            }
            if( interact == C && Can_Change )     /* C - Hold */
            {
                if( Hold.ReturnType() == None )  /* Fisrt Hold */
                {
                    Hold = Cur ;
                    next = true ;
                }
                else                             /* Swap Hold with Current */
                {
                    std::swap( Hold , Cur ) ;
                }

                for( int i = 1 ; i < BoardSize_X - 1 ; i ++ )
                    for( int j = 1 ; j < BoardSize_Y - 1 ; j ++ )
                        if( board.Show(i,j).ReturnType() == Shape  )
                            board.Show(i,j).Init() ;

                buffer.Init(false) ;

                Can_Change = false ;
                break ;
            }

            Showpect( &board , &buffer ) ;
            if( interact != 0 ) ShowBoard( &board ) ;   // Refrash

        }

        if( next )
        {
            EraseLine( &board , &Score , &Line , &Combo ) ;
            Cur = Next , Next.GainBlock() ;
            Can_Change = true ;
        }
    }
    while( !GameOver(&buffer) ) ;

    board.Destroy() , buffer.Destroy() ;
    Clean();
    FileSave( Score ) ;
}

void ShowRecord()
{
    PA* data = FileRead() ;

    Clean();
    Cursorset( "  R A N K\t    P L A Y E R\t          S C O R E                 D A T E     " ) ;
    for( int i = 0 ; i < log_num ; i ++ )
    {
        if( (data+i)->Score != -1 )
            printf( "  Number %2d :\t     %10s\t        %9d                 %s\n" , i + 1 , (data+i)->Name , (data+i)->Score , (data+i)->Date ) ;
    }
    if( data->Score == -1 ) printf( "\n\n\n\n\n\n\n\n\n\n\n\t\t\tNo records , Please enjoy the Game~\n" ) ;
    gotoxy(0,18);
    printf("                                          \n");     /* Cover previous Screen */
    Draw( 38 , 23 , "[OK]" , true ) ;
    free(data);                         /* Free Malloc */
    while( KeyPressDetect() != Enter ) ;

}
void ShowBoard( GameBoard* gb )
{
    gb->Print(27,3);
}
void ShowForm( GameBoard* gb )
{
    for( int i = 0 ; i < BoardSize_X ; i ++ ) /* Main Form */
    {
        gotoxy( 25 , 2 + i ) ;
        for( int j = 0 ; j < BoardSize_Y ; j ++ )
            gb->Show(i,j).Print() ;

    }
    for( int i = 0 ; i < 7 ; i ++ )
    {
        gotoxy( 52 , 2+i ) ;    /* Next Form */
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

        gotoxy( 10 , 2+i ) ;    /* Hold Form */
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
        gotoxy( 54 , 4+i ) ;
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
            gotoxy( 12 , 4+i ) ;
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
    gotoxy( 52 , 11 ) ;
    printf( "Score : %d " , total_score ) ;
    gotoxy( 52 , 13 ) ;
    printf( "Cleared Line : %d " , total_line ) ;
    gotoxy( 52 , 15 ) ;
    printf( "Combo : %d " , combo ) ;

}
void EraseLine( GameBoard* gb , int* score , int* line , int* combo )
{
    int add = 0 , ers [4] ;
    int i , j ;   /* For loop */

    for( i = BoardSize_X-2 ; i > 1 ; i -- )
    {
        int check = 0 ;
        for( j = 1 ; j < BoardSize_Y - 1 ; j ++ )
        {
            if( gb->Show(i,j).ReturnType() == Lock ) check ++ ;
        }

        if( check == BoardSize_Y - 2 )  ers[add++] = i + add ;
    }
    for( int i = 0 ; i < add ; i ++ )
    {
        for( int j = ers[i]-1 ; j > 0 ; j -- )
        {
            for( int k = 1 ; k < BoardSize_Y - 1 ; k ++ )
                MoveBlock( gb , j , k , j+1 , k ) ;
        }
    }
    *score += Lscore[add]*(1+*combo) ;
    *line += add ;
    if( add == 0 ) *combo = 0 ;
    else *combo += 1 ;
}
bool GameOver( GameBoard* buffer )
{
    return !buffer->IsEmpty() ;
}
