// ******************************************************************************************* //
// File:         lab2p2.c
// Date:
// Authors:     Steven Wirth
//
// Description:
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>
#include <string.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )



typedef enum stateTypeENUM{
    ENTER, GOOD, VALID, BAD, SETMODE, INVALID, WAIT, DEBOUNCE, WRITE, WAITRELEASE, CHECK, SETCHECK
} stateType;

int CursorCounterX;             //Keeps Track of Cursor Postion X-Coordinate
int CursorCounterY;             //Keeps Track of Cursor Postion Y-Coordinate
volatile int inputcounter;      //Keeps Track of number of input characters
char checkChar;                 //Holds the character that is being scanned in
char input [4];                 //Array of the currently inputted words
char pw1[4][4];                 //4x4 array that holds 4 passwords of length 4 each

int i = 0;                      // Used for looping through arrays
int starcounter = 0;            //keeps track of how many stars there are (initally to check that the first two characters being stars puts it into set mode

volatile stateType currstate;   //Holds the current state that the circuit is in
volatile stateType nextState;   //Holds the next state to go to in the logic machine
volatile stateType prevState;   //Holds the previous state to go to in the logic machine
volatile int timerstarted;      //Flag for if the timer was started or not
volatile int secondscounter;    //Holds the counter for the amount of seconds passed
volatile int passcounter;       //The Counter for which password we're currently replacing
volatile int setflag;           //Flag for whether the data is set or not


int main(void)
{
//    /*
//     I initialize all the counters and variables and I set the starting state
//     as the "Enter" state. I also added a default password of "1234" to the
//     * stored passwords.
//
//    */
    initLCD();
    initKeypad();
    
    CursorCounterX = 0;
    CursorCounterY = 0;
    inputcounter = 0;
    timerstarted = 0;
    secondscounter = 0;
    passcounter = 1;
    setflag =0;

    strcpy(pw1[0],"1234");
    strcpy(pw1[1],"    ");
    strcpy(pw1[2],"    ");
    strcpy(pw1[3],"    ");      


   
    currstate = ENTER;


    while(1){

        switch (currstate)
        {
            //Since the enter case is the state that all sequences lead back to, we use this to
            //Reinitialize all the counters and variables needed throughout the sequence.
            //As well clearing the LCD and displays enter. Switches to the WAIT state to
            //Wait for key presses
            case ENTER:
                clearLCD();
                delayUs(1640);
                CursorCounterY = 0;
                CursorCounterX = 0;
                timerstarted = 0;
                inputcounter = 0;
                secondscounter = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                delayUs(40);
                printStringLCD("ENTER:");
                CursorCounterY = 1;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                delayUs(40);
                currstate = WAIT;
                prevState=ENTER;
                break;

            //Waits for a button to be pressed then goes to the appropriate state.
            case WAIT:
                
                currstate = WAIT;
                
                if (inputcounter == 4 && setflag == 0) // MAX ENTRY SO WE NOW NEED TO CHECK IF VALID
                    {
                        prevState = WAIT;
                        currstate = CHECK;
                    }
                else if (inputcounter == 2 && (input[1] == '*') && (input[1] == '*'))
                {
                    currstate = SETMODE;
                    clearLCD();
                    delayUs(1640);
                }
                else if (inputcounter == 4 && setflag == 1) // MAX ENTRY SO WE NOW NEED TO CHECK IF VALID
                    {
                        prevState = WAIT;
                        currstate = SETCHECK;
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
                if (timerstarted == 0)
                {
                    startTime();
                    timerstarted = 1;
                }
                break;
            case BAD:
                prevState = CHECK;
                currstate = BAD;
                nextState = ENTER;
                CursorCounterY = 0;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                printStringLCD("BAD");             
                if (timerstarted == 0)
                {
                    startTime();
                    timerstarted = 1;
                }
                break;
            case CHECK:
                i = 0;
                starcounter = 0;
                nextState = GOOD;
                for(i=0;i<inputcounter;i++)
                {
                   if(input[i] == '#')
                   {
                       nextState = BAD;
                   }
                   else if(((i == 0)||(i==1)) && (input[i] == '*'))
                   {
                       starcounter++;
                   }
                   
                   else if((input[i] != pw1[0][i]) && (input[i] != pw1[1][i]) && (input[i] != pw1[2][i]) && (input[i] != pw1[3][i]))
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
                clearLCD();
                delayUs(1640);
                break;
            case SETMODE:
                CursorCounterY = 0;
                CursorCounterX = 0;
                timerstarted = 0;
                inputcounter = 0;
                secondscounter = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                delayUs(40);
                printStringLCD("SET MODE");
                CursorCounterY = 1;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                delayUs(40);
                currstate = WAIT;
                prevState=SETMODE;
                setflag =1;
                break;
            case VALID:
                prevState = SETCHECK;
                currstate = VALID;
                nextState = ENTER;
                CursorCounterY = 0;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                printStringLCD("VALID");
                if (timerstarted == 0)
                {
                    startTime();
                    timerstarted = 1;
                }
                break;
            case INVALID:
                prevState = SETCHECK;
                currstate = INVALID;
                nextState = ENTER;
                CursorCounterY = 0;
                CursorCounterX = 0;
                moveCursorLCD(CursorCounterY, CursorCounterX);
                printStringLCD("INVALID");
                if (timerstarted == 0)
                {
                    startTime();
                    timerstarted = 1;
                }
                break;
            case SETCHECK:
                i = 0;
                nextState = VALID;
                for(i=0;i<4;i++)
                {
                   if(input[i] == '#' || input[i] == '*')
                   {
                       nextState = INVALID;
                   }

                }
                if(nextState != INVALID)
                {
                    for(i=0;i<4;i++)
                    {
                       pw1[passcounter][i] = input[i];

                    }
                    
                    //strcpy(pw1[passcounter], input);
                    passcounter++;
                }
                
                currstate = nextState;
                prevState = SETCHECK;
                clearLCD();
                delayUs(1640);
                setflag = 0;
                break;
        }
    }

    return 0;
}

void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;

    if(currstate == WAIT)
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
        currstate = nextState;
        T1CONbits.TON = 0;
    }
    else
    {
        secondscounter++;
        startTime();

    }


}
