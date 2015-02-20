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
    RUN, STOP, dp, dp2, dr, dr2, CLEAR
} stateType;

volatile stateType currstate;
volatile int counter;
volatile int insw;
volatile int exsw;

int main(void) {

    currstate = STOP;
    insw = 0;
    exsw = 0;

    while(1)
    {
        switch(currstate)
        {
            case RUN:
                LED_STOP = OFF;
                LED_RUN = ON;
                printStringLCD("RUNNING:");
                moveCursorLCD(1,0);
                //printStringLCD(getTimeString());
                break;
            case STOP:
                LED_RUN = OFF;
                LED_STOP = ON;
                printStringLCD("STOPPED:");
                moveCursorLCD(1,0);
                //printStringLCD(getTimeString());
                break;
            case dp:
                Timer1Delay(5);
                currstate = RUN;
                break;
            case dp2:
                Timer1Delay(5);
                currstate = STOP;
                break;
            case dr:
                Timer1Delay(5);
                currstate = RUN;
                clearLCD();
                moveCursorLCD(0,0);
                break;
            case dr2:
                Timer1Delay(5);
                currstate = STOP;
                clearLCD();
                moveCursorLCD(0,0);
                break;
            case CLEAR:
                LED_RUN = OFF;
                LED_STOP = ON;
                break;

        }
    }

    return 0;
}

void _ISR _CNInterrupt(void)
{
    IFS1bits.CNIF = 0;

    if(PORTBbits.RB2 == PRESSED)
    {
       exsw = 1;
    }
    else if(PORTBbits.RB5 == PRESSED)
    {
        insw = 1;
    }

    if(insw == 1 && PORTBbits.RB5 == RELEASED && currstate == RUN)
    {
        currstate = currstate;
    }
    else if(insw == 1 && PORTBbits.RB5 == RELEASED && currstate == STOP)
    {
        currstate = STOP;
    }



}

void _ISR _T1Interrupt(void)
{
}
