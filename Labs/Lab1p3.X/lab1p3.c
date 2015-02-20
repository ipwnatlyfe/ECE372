/*
 * File:   lab1p3.c
 * Author: Jake
 *
 * Created on February 19, 2015, 5:24 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "p24fj64ga002.h"
#include "cn.h"
#include "leds.h"
#include "lcd.h"
#include "timer.h"



_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )


// ******************************************************************************************* //

          typedef enum stateTypeENUM{
    RUN, WAIT, DBP, DBR, CLEAR, INIT
} stateType;

volatile stateType currstate;
volatile int counter;
volatile int insw; //Flag for internal switch RB5
volatile int exsw; //Flag for internal switch RB2
volatile unsigned int TIMERCOUNTER=0; //Counts how many milliseconds


int main(void) {

    initCNForSW1();
    initLEDs();
    initSW2();
    currstate = INIT;
    insw = 0;
    exsw = 0;

    while(1)
    {
        switch(currstate)
        {
            case INIT:
                initLCD();
                currstate = WAIT;
                break;

            case RUN:
                LED_STOP = OFF;
                LED_RUN = ON;
                printStringLCD("RUNNING:");             
                moveCursorLCD(1,0);
                printStringLCD(getTimeString(TIMERCOUNTER));
                moveCursorLCD(0,0);
                break;

            case WAIT:
                LED_RUN = OFF;
                LED_STOP = ON;
                printStringLCD("STOPPED:");
                moveCursorLCD(1,0);
                printStringLCD(getTimeString(TIMERCOUNTER));
                moveCursorLCD(0,0);
                break;
           /* case DBP:
                Timer1Delay(5);
                currstate = RUN;
                break;
            *
            case DBR:
                Timer1Delay(5);
                currstate = RUN;
                clearLCD();
                moveCursorLCD(0,0);
                break;
            * */
            case CLEAR:
                LED_RUN = OFF;
                LED_STOP = ON;
                TIMERCOUNTER = 0;
                clearLCD();
                currstate = WAIT;
                break;

                
        }
    }

    return 0;
}

void _ISR _CNInterrupt(void)
{
    IFS1bits.CNIF = 0;
    Timer1Delay(5);
    if(PORTBbits.RB2 == PRESSED)
    {
       exsw = 1;
    }

    else if(PORTBbits.RB5 == PRESSED)
    {
        insw = 1;
    }
    

    if(insw == 1 && PORTBbits.RB5 == RELEASED && currstate == WAIT)
    {
        currstate = CLEAR;
        insw = 0;
    }
    else if(exsw == 1 && PORTBbits.RB2 == RELEASED && currstate == WAIT)
    {
        currstate = RUN;
        startTime();
        exsw = 0;
    }
    else if(exsw == 1 && PORTBbits.RB2 == RELEASED && currstate == RUN)
    {
        currstate = WAIT;
        exsw = 0;
    }



}

void __attribute__((__interrupt__, __shadow__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    
    if(currstate == RUN)
    {
       TIMERCOUNTER++;
       TMR1 = 0;
    }
    else
    {
        T1CONbits.TON = 0;
    }


}
