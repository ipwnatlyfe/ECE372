// ******************************************************************************************* //
//
// File:         lab1p1.c
// Date:         
// Authors:      
//
// Description: Part 1 for lab 1
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "leds.h"
#include "timer.h"
#include <stdio.h>

#define PRESSED 0
#define RELEASED 1

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //
          typedef enum stateTypeENUM{
    wfp, dp, wfr, dr
} stateType;

volatile stateType currstate;

int main(void)
{
    initLEDs();
    initSW2();

    currstate = wfp;

    LED_RUN = ON;
    LED_STOP = OFF;

    while(1)
    {
        //TODO: Using a finite-state machine, define the behavior of the LEDs
        //Debounce the switch
        switch(currstate)
        {
                case wfp:
                    break;
                case dp:
                    Timer1Delay(5);
                    currstate = wfr;
                    break;
                case wfr:
                    break;
                case dr:
                    Timer1Delay(5);
                    currstate = wfp;
                    break;

        }
    }
    
    return 0;
}

void _ISR _CNInterrupt(void){
    //TODO: Implement the interrupt to capture the press of the button
    IFS1bits.CNIF = 0;

    if(currstate == wfp)
    {
        currstate = dp;
    }
    else if(currstate == wfr)
    {
        if(PORTAbits.RA0 == ON)
        {
            LED_RUN = OFF;
            LED_STOP = ON;
        }
        else if(PORTAbits.RA1 == ON)
        {
            LED_STOP = OFF;
            LED_RUN = ON;
        }
        currstate = dr;
    }
}
