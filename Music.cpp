#include <windows.h>
#include <mmsystem.h>


void StopMusic()
{
    PlaySound(NULL, 0, 0);
}
void MenuMusic()
{
    PlaySound( TEXT("Terra.wav") , NULL , SND_FILENAME | SND_ASYNC | SND_LOOP ) ;
}
void PlayMusic()
{
    PlaySound( TEXT("LOL.wav") , NULL , SND_FILENAME | SND_ASYNC | SND_LOOP ) ;
}
