#ifndef LOCATION_H_INCLUDED
#define LOCATION_H_INCLUDED

#define S_Form_X 25
#define S_Form_Y 2
#define S_Next_X 52
#define S_Next_Y 2
#define S_Hold_X 10
#define S_Hold_Y 2
#define S_Score_X 52
#define S_Score_Y 11
#define S_Pause_X 27
#define S_Pause_Y 13

#define M_Form_X 17
#define M_Form_Y 2
#define M_Next_X 2
#define M_Next_Y 10
#define M_Hold_X 2
#define M_Hold_Y 2
#define M_Score_X 3
#define M_Score_Y 18
#define M_Pause_X 19
#define M_Pause_Y 13

int Form_X = 17 ;
int Form_Y = 2 ;
int Next_X = 2 ;
int Next_Y = 10 ;
int Hold_X = 2 ;
int Hold_Y = 2 ;
int Score_X = 3 ;
int Score_Y = 18 ;
int Pause_X = 19 ;
int Pause_Y = 13 ;

int Form_X2 = 47 ;
int Form_Y2 = 2 ;

void Change_Mode( Mode M )
{
    if( M == Single ){
        Form_X = S_Form_X ;
        Form_Y = S_Form_Y ;
        Next_X = S_Next_X ;
        Next_Y = S_Next_Y ;
        Hold_X = S_Hold_X ;
        Hold_Y = S_Hold_Y ;
        Score_X = S_Score_X ;
        Score_Y = S_Score_Y ;
        Pause_X = S_Pause_X ;
        Pause_Y = S_Pause_Y ;
    }
    else{
        Form_X = M_Form_X ;
        Form_Y = M_Form_Y ;
        Next_X = M_Next_X ;
        Next_Y = M_Next_Y ;
        Hold_X = M_Hold_X ;
        Hold_Y = M_Hold_Y ;
        Score_X = M_Score_X ;
        Score_Y = M_Score_Y ;
        Pause_X = M_Pause_X ;
        Pause_Y = M_Pause_Y ;
    }
}


#endif // LOCATION_H_INCLUDED
