#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Console.h"
#include "Color.h"

struct PA{

    char Name[15] ;
    int Score ;
    char Date[15] ;
} ;

void Prod_TXT() ;
int Encode( int pwd ) ;
int Decode( int pwd ) ;
char* GetTime( ) ;

const char* FileName = ".\\Record" ;  /*    Direct Path    */
const int log_num = 15 ;              /* Number of Records */
const int propwd = 17 ;               /* Password  codenum */

PA* FileRead()
{
    Prod_TXT();
    PA* Data = (PA*)malloc( log_num*sizeof(PA) ) ;
    FILE* fp = fopen( FileName , "r" ) ;

    for( int i = 0 ; i < log_num ; i ++ )
    {
        fscanf( fp , "%s%d%s" , (Data+i)->Name , & (Data+i)->Score , (Data+i)->Date ) ;
        (Data+i)->Score = Decode((Data+i)->Score ) ;
    }
    fclose(fp) ;
    return Data ;
}
void FileSave( int score )
{
    PA* Data = FileRead( ) ;
    int i = -1 ;
// **************** Compare ******************* //
    while(  ++i < log_num && (Data+i)->Score >= score ) ;         /* Search Ranking */

    if( i < log_num )
    {
        for( int j = log_num - 1 ; j > i ; j -- )
        {
            (Data+j)->Score = (Data+j-1)->Score ;
            strcpy( (Data+j)->Name , (Data+j-1)->Name ) ;
            strcpy( (Data+j)->Date , (Data+j-1)->Date ) ;
        }
        (Data+i)->Score = score ;
        printf( "You are number %d !! \nPlease input your name ... > " , i + 1 ) ;
        scanf( "%s" , (Data+i)->Name ) ;
        strcpy( (Data+i)->Date , GetTime() ) ;
// ***************** Saving ******************** //
    FILE* fp = fopen( FileName , "w" ) ;
    for( int j = 0 ; j < log_num ; j ++ )
        fprintf( fp , "%s %d %s\n" , (Data+j)->Name , Encode( (Data+j)->Score ) , (Data+j)->Date  ) ;

        fclose(fp) ;
    }

    free( Data ) ;
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

void Prod_TXT()
{
    if( fopen( FileName , "r" )  == NULL )  /* New player , Make a record file */
    {
        FILE *re = fopen( FileName , "w" ) ;
        for( int i = 0 ; i < log_num ; i ++ )
        {
            fprintf( re , "None %d None\n" , Encode( -1 ) ) ;
        }
        fclose(re) ;
    }
}
char* GetTime()
{
    time_t now ;
    time(&now) ;
    struct tm* NowTime = localtime(&now) ;
    char fmtbuf[30];
    strftime(fmtbuf, sizeof( fmtbuf ), "%m/%d/%Y", NowTime );
    return fmtbuf ;
}
int Encode( int pwd )
{
    union
    {
        int num ;
        char c[sizeof(int)] ;
    } u1 ;

    u1.num= pwd ;
    for( int i = 0 ; i < sizeof(int) ; i ++ ) u1.c[i] += propwd ;

    return u1.num ;
}
int Decode( int pwd )
{
    union
    {
        int num ;
        char c[sizeof(int)] ;
    } u2 ;

    u2.num= pwd ;
    for( int i = 0 ; i < sizeof(int) ; i ++ ) u2.c[i] -= propwd ;

    return u2.num ;
}
