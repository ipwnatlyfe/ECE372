// ******************************************************************************************* //
// File:         lab2p1.c
// Date:         
// Authors:      
//
// Description: 
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )



typedef enum stateTypeENUM{
    WAIT, DEBOUNCE, START, WAITRELEASE
} stateType;

volatile stateType currstate;
volatile stateType nextState;
volatile stateType prevState;
volatile int counter;
volatile int counter2;
volatile int set;
char checkChar;


int main(void)
{
    initLCD();
    initKeypad();
    currstate = WAIT;

    counter=0;
    counter2=0;
    set = 0;
    while(1){
       
        
        switch (currstate)
        {
            case WAIT:
                prevState = DEBOUNCE;
                nextState = DEBOUNCE;
                set = 1;
                break;
            case DEBOUNCE:
                delayUs(800);
                nextState = START;               
                if(prevState == WAITRELEASE)
                {
                    currstate = WAIT;
                }
                else
                {
                prevState = DEBOUNCE;
                currstate = nextState;
                nextState = WAITRELEASE;
                }
                break;
            case START:
                checkChar = scanKeypad();
                if(checkChar != -1)
                {
                    printCharLCD(checkChar);                   
                    counter++;
                    if(counter == 8 )
                    {
                        if(counter2==1)
                        {
                            counter2 = 0;
                        }
                        else
                        {
                        counter2++;
                        }
                        counter = 0;
                        moveCursorLCD(counter2,counter);
                    }

                    prevState = START;
                    currstate = WAITRELEASE;
                }
                else
                {
                    prevState = START;
                    currstate = WAITRELEASE;
                }
                break;
            case WAITRELEASE:
                currstate == WAITRELEASE;
                break;
        }
    }
    
    return 0;
}

void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;

    if(currstate == WAIT)
    {
        if(set == 1)
        {
        currstate = nextState;
        prevState = WAIT;
        }
    }
    else if(currstate == WAITRELEASE)
    {
        nextState = DEBOUNCE;
        currstate = nextState;
        prevState = WAITRELEASE;
    }

    return;
}