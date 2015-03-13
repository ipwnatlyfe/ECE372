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
    ENTER, GOOD, VALID, BAD, SETMODE, INVALID, WAIT, DEBOUNCE, WRITE, WAITRELEASE, CHECK
} stateType;

int CursorCounterX;
int CursorCounterY;
volatile int inputcounter;
char checkChar;
char input [4];
char pw1 [4] = "    ";
char pw2 [4] = "    ";
char pw3 [4] = "    ";
char pw4 [4] = "    ";

int i = 0;
int starcounter = 0;

volatile stateType currstate;
volatile stateType nextState;
volatile stateType prevState;
volatile int timerstarted = 0;
volatile int secondscounter = 0;


int main(void)
{
    initLCD();
    initKeypad();
    
    CursorCounterX = 0;
    CursorCounterY = 0;
    inputcounter = 0;

    currstate = ENTER;


    while(1){

        switch (currstate)
        {
            case ENTER:
                CursorCounterY = 0;
                CursorCounterX = 0;
                timerstarted = 0;
                inputcounter = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                printStringLCD("ENTER:");
                CursorCounterY = 1;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                currstate = WAIT;
                break;
            case WAIT:
                prevState=ENTER;
                currstate = WAIT;
                if(prevState == WAITRELEASE)
                {
                    if (inputcounter == 3) // MAX ENTRY SO WE NOW NEED TO CHECK IF VALID
                    {
                        prevState = WAIT;
                        currstate = CHECK;
                    }
                }
                break;
            case DEBOUNCE:
                delayUs(800);
                nextState = WRITE;
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
            case WRITE:                
                checkChar = scanKeypad();
                if(checkChar != -1)
                {
                    printCharLCD(checkChar);
                    input[inputcounter] = checkChar;
                    inputcounter++;

                    prevState = WRITE;
                    currstate = WAITRELEASE;
                }

                else
                {
                    prevState = WRITE;
                    currstate = WAITRELEASE;
                }
                break;
            case WAITRELEASE:
                break;

            case GOOD:
                prevState = CHECK;
                currstate = GOOD;
                nextState = ENTER;
                CursorCounterY = 0;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                printStringLCD("GOOD");
                CursorCounterY = 1;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                printStringLCD("    ");
                if (timerstarted == 0)
                {
                    startTime();
                    timerstarted = 1;
                }

            case BAD:
                prevState = CHECK;
                currstate = BAD;
                nextState = ENTER;
                CursorCounterY = 0;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                printStringLCD("BAD");
                CursorCounterY = 1;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                printStringLCD("    ");
                if (timerstarted == 0)
                {
                    startTime();
                    timerstarted = 1;
                }

            case CHECK:
                i = 0;
                starcounter = 0;
                nextState = GOOD;
                for(i=0;i<inputcounter;i++)
                {
                   if(input[i] = '#')
                   {
                       nextState = BAD;
                   }
                   else if((((i == 0)||(i==1)) && input[i]) == '*')
                   {
                       starcounter++;
                   }
                   
                   else if((input[i] != pw1[i]) || (input[i] != pw1[i]) || (input[i] != pw1[i]) || (input[i] != pw1[i]))
                   {
                       nextState = BAD;
                   }

                }
                if(starcounter == 2)
                {
                    nextState = SETMODE;
                }
                currstate = nextState;
                prevState = CHECK;

        }
    }

    return 0;
}

void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;

    if(currstate = WAIT)
    {
        prevState = WAIT;
        currstate = DEBOUNCE;

    }
    else if(currstate == WAITRELEASE)
    {
        nextState = DEBOUNCE;
        currstate = nextState;
        prevState = WAITRELEASE;
        
    }


}

void __attribute__((__interrupt__, __shadow__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;

    if (secondscounter == 2)
    {
        secondscounter = 0;
        currstate = nextState;
    }
    else
    {
        secondscounter++;
        startTime();

    }


}
