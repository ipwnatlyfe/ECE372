#include "cn.h"


void startcheckButton()
{
    if(PORTBbits.RB2 == PRESSED)
    {
       exsw = 1;
    }

    else if(PORTBbits.RB5 == PRESSED)
    {
        insw = 1;
    }

    
}
