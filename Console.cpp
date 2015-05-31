#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Color.h"

HANDLE Screen_Out  = GetStdHandle(STD_OUTPUT_HANDLE) ;    // Standard Output -> Screen

int KeyPressDetect()
{
    if ( kbhit( ) ) /* kbhit() -> if have input */
    {
        return (int)getch() ;
    }
    return 0 ;
}
void gotoxy( int x , int y )
{
    COORD dot = { x , y } ;
    SetConsoleCursorPosition( Screen_Out , dot );
}
//****************************************
void ColorText( int choose )
{
    if( choose == Red ) SetConsoleTextAttribute( Screen_Out , RED ) ;
    else if( choose == Green ) SetConsoleTextAttribute( Screen_Out , GREEN ) ;
    else if( choose == Blue ) SetConsoleTextAttribute( Screen_Out , BLUE ) ;
    else if( choose == Yellow ) SetConsoleTextAttribute( Screen_Out , YELLOW ) ;
    else if( choose == Purple ) SetConsoleTextAttribute( Screen_Out , PURPLE ) ;
    else if( choose == Cyan ) SetConsoleTextAttribute( Screen_Out , CYAN ) ;
    else if( choose == White ) SetConsoleTextAttribute( Screen_Out , WHITE ) ;
    else if( choose == Red+num_of_color ) SetConsoleTextAttribute( Screen_Out , RED_BACK ) ;
    else if( choose == Green+num_of_color ) SetConsoleTextAttribute( Screen_Out , GREEN_BACK ) ;
    else if( choose == Blue+num_of_color ) SetConsoleTextAttribute( Screen_Out , BLUE_BACK ) ;
    else if( choose == Yellow+num_of_color ) SetConsoleTextAttribute( Screen_Out , YELLOW_BACK ) ;
    else if( choose == Purple+num_of_color ) SetConsoleTextAttribute( Screen_Out , PURPLE_BACK ) ;
    else if( choose == Cyan+num_of_color ) SetConsoleTextAttribute( Screen_Out , CYAN_BACK ) ;
    else if( choose == White+num_of_color ) SetConsoleTextAttribute( Screen_Out , WHITE_BACK ) ;
}
//*****************************************
void Cursorset( char t[] )
{
    ColorText(Yellow+num_of_color) ;
    printf( "%s" , t ) ;
    ColorText(White) ;
}
void Drawtext( char t[] , int color )
{
    ColorText(color);
    printf( "%s" , t ) ;
    ColorText(White);
}
void Draw( int x , int y , char t[] , bool c_set )
{
    gotoxy( x , y ) ;
    if( c_set ) Cursorset( t ) ;
    else Drawtext( t , White ) ;
}
void DrawBlock( int bcolor )
{
    ColorText(bcolor);
    printf( "¢i" ) ;
    ColorText(White);
}
//*****************************************
void Clean()  // For windows
{
    CONSOLE_SCREEN_BUFFER_INFO sbinf;
    int size;
    COORD scrn = {0, 0};
    DWORD temp;
    GetConsoleScreenBufferInfo(Screen_Out, &sbinf);
    size = (sbinf.srWindow.Right-sbinf.srWindow.Left+1)*
           (sbinf.srWindow.Bottom-sbinf.srWindow.Top+1);
    FillConsoleOutputCharacter(Screen_Out, 32, size, scrn, &temp);
    gotoxy(0,0);
}
void pause()
{
    system( "pause" ) ;
}
void delay( int ms )
{
    Sleep(ms);
}
int random( int ln , int un )
{
    return rand()%(un-ln+1) + ln ;
}
