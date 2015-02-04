// ******************************************************************************************* //
//
// File:         lab0.c
// Date:         
// Authors:      
//
// Description:  Lab 0 assignment for ECE 372 Spring 2015.
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "initLab0.h"
#include <stdio.h>

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
          BKBUG_ON & COE_ON & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )



typedef enum stateTypeEnum{
    //TODO: Define states by name
    LED_OFF, LED_ONE, LED_TWO, LED_THREE, LED_FOUR
} stateType;

#define PRESSED 0
#define NOTPRESSED 1

stateType CurrentState = LED_OFF;
stateType NextState = LED_ONE;
stateType BackState = LED_FOUR;


int main(void)
{

;

    //TODO: Finish these functions in the provided c files
    initLEDs();
    initTimer1();
    initSW1();
    
    while(1)
    {
        //Use a switch statement to define the behavior based on the state
//        switch(){
//
//        }
        switch (CurrentState){
            case LED_ONE:
                BackState = LED_FOUR;
                NextState = LED_TWO;
                LATBbits.LATB12 = 0;
                LATBbits.LATB15 = 1;
                LATBbits.LATB13 = 1;
            break;

            case LED_TWO:
                BackState = LED_ONE;
                NextState = LED_THREE;
                LATBbits.LATB13 = 0;
                LATBbits.LATB12 = 1;
                LATBbits.LATB14 = 1;
            break;

            case LED_THREE:
                BackState = LED_TWO;
                NextState = LED_FOUR;
                LATBbits.LATB14 = 0;
                LATBbits.LATB13 = 1;
                LATBbits.LATB15 = 1;
            break;

            case LED_FOUR:
                BackState = LED_THREE;
                NextState = LED_ONE;
                LATBbits.LATB15 = 0;
                LATBbits.LATB14 = 1;
                LATBbits.LATB12 = 1;
            break;
        }


    }
    return 0;
}

void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;      //Reset Change Notifaction Flag
    T2CONbits.TON = 1;      //Start Timer when the buttom is pushed
                            //If the button is not released, it should take us to the timer interrupt
    if(PORTBbits.RB5 == NOTPRESSED)  //If the switch is released, go to next light
    {
        T2CONbits.TON = 0;
        TMR2 = 0;
        TMR3 = 0;
        //The Previous three lines turn the timer off and reset the values for it
        CurrentState = NextState;
    }

}
void _ISR _T3Interrupt(void){
    //TODO: Put down the timer 3 flag first!
    IFS0bits.T3IF = 0;
    T2CONbits.TON = 0;
    TMR2 = 0;
    TMR3 = 0;
    //TODO: Change states if necessary.
    //Make sure if you use any variables that they are declared volatile!
    CurrentState = BackState;

}