/*
* File:   initLab0.c
* Author:
*
* Created on December 27, 2014, 1:31 PM
*/

#include "p24fj64ga002.h"
#include "initLab0.h"

#define PRVAL     29491199  //PR Value for 32 bit timer for 2seconds
#define TIMERLOW  65535 //Lower 16 bits put in timer 2
#define TIMERHIGH 449   //Upper 16 bits put in timer 3



void initLEDs(){
    //TODO: Initialize the pin connected to the LEDs as outputs
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB15 = 0;
    //TODO: Turn each LED OFF
    LATBbits.LATB12 = 1;
    LATBbits.LATB13 = 1;
    LATBbits.LATB14 = 1;
    LATBbits.LATB15 = 1;

}

void initSW1(){
    //TODO: Initialize the pin connected to the switch as an input.
    TRISBbits.TRISB5 = 1; // pin 14, switch 1, input
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    CNEN2bits.CN27IE = 1;
}


void initTimer1(){
    //TODO: Initialize the timer
    T2CONbits.T32 = 1;
    T2CONbits.TCKPS = 0b00;
    PR3 = 449;
    PR2 = 65535;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;              // Put the interrupt flag down
   // T2CONbits.TON = 1;            // turn timer 2 on



}
