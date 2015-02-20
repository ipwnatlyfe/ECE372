/*
* File:   leds.c
* Author: 
*
* Created on December 27, 2014, 1:31 PM;
*/

#include "p24fj64ga002.h"
#include "leds.h"


void initLEDs(){

    //TODO: Initialize the appropriate pins to work with the LEDs
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    
    AD1PCFGbits.PCFG0 = 0;
    AD1PCFGbits.PCFG1 = 0;

    ODCAbits.ODA0 = 1;
    ODCAbits.ODA1 = 1;

    LED_STOP = OFF;
    LED_RUN = OFF;

}

void initSW2(){
    //TODO: Initialize the appropriate pins to work with the external switch using a
    //change notification interrupt.
    TRISBbits.TRISB2 = 1;
    CNEN1bits.CN6IE = 1;
    CNPU1bits.CN6PUE = 1;
    AD1PCFGbits.PCFG4 = 1;
    IEC1bits.CNIE = 1;
    IFS1bits.CNIF = 0;
}