#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

enum Key { Enter = 13 , Up = 72 , Down = 80 , Left = 75 , Right = 77 , Space = 32 , C = 99 , Pause = 112 } ;

int KeyPressDetect() ;
void gotoxy( int x , int y ) ;
void ColorText( int color ) ;
void ColorBackGround ( int color ) ;
void Cursorset( char t[] ) ;
void Draw( int x , int y , char t[] , bool c_set ) ;
void Drawtext( char t[] , int color ) ;
void DrawBlock( int bcolor ) ;
void Clean() ;
void pause() ;
void delay( int ms ) ;
int random( int ln , int un ) ;

#endif // CONSOLE_H_INCLUDED
