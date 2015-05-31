#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

extern int log_num ;

struct PA{

    char Name[15] ;
    int Score ;
    char Date[15] ;
} ;

PA* FileRead() ;
void FileSave(int) ;
void Prod_TXT() ;
char* GetTime( ) ;
int Encode( int pwd ) ;
int Decode( int pwd ) ;

#endif // FILE_H_INCLUDED
