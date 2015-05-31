#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Console.h"
#include "Game.h"
#include "Color.h"

bool PlayAgain( ) ;
int welcome() ;
void PrintLogo() ;

int main()
{
    srand( time(NULL) ) ;
    int state ;
    do
    {
        Clean();
        switch ( state = welcome() )
        {
            case 1 :
                Game() ;
            case 2 :
                ShowRecord() ;
                break ;
            case 3 :
                return 0 ;
        }
    }
    while( state != 3 );

    return 0 ;
}
int welcome()
{
    int state = 1 , key = 0 , prekey = -1 ;
    do
    {
        key = KeyPressDetect() ;
        if( key == Up && state != 1 ) state -- ;
        if( key == Down && state != 3 ) state ++ ;
        if( key == prekey ) continue ;
        prekey = key ;

        PrintLogo() ;
        Draw( 30 , 15 , "(1) Game Start!!" , state == 1 ) ;
        Draw( 32 , 18 , "(2) Record" , state == 2 ) ;
        Draw( 33 , 21 , "(3) Exit\n" , state == 3 ) ;

        printf("\n                                                            ") ;
    }
    while( key != Enter ) ;

    return state ;
}
void PrintLogo()
{
    /* long : 40 , high : 12 */

    bool Logo[12][40] ={ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1},
                        {0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1},
                        {0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0},
                        {0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1},
                        {0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1},
                        {0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1},
                        {0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1},
                        {0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                      } ;
    gotoxy(0,0);
    for( int i = 0 ; i < 12 ; i ++ )
        for( int j = 0 ; j < 40 ; j ++ )
            if( Logo[i][j] ) DrawBlock(Yellow);
            else printf("  ");

    for( int i = 0 ; i < 4 ; i ++ )
    {
        gotoxy( 10 , 15+i ) ;
        for( int j = 0 ; j < 4 ; j ++ )
            if( Bricks[7][i*4+j] )
               DrawBlock( Green ) ;
            else
                printf( "  " ) ;
    }


}
