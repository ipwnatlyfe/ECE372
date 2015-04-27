/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include "p24fj64ga002.h"
#include "timer.h"
#include <stdio.h>

#define FCY 14745600
#define PRESCALAR 256
#define ONE_MILLISECOND 57
#define MIN_TO_MS 60000
#define SEC_TO_MS 1000

//Uses timer 2
void delayUs(unsigned int delay){
    //TODO: Create a delay using timer 2 for "delay" microseconds.
    TMR2 = 0;
    PR2 = delay*14;
    IFS0bits.T2IF = 0;
    T2CONbits.TCKPS = 0;
    T2CONbits.TON = 1;
    while(IFS0bits.T2IF == 0);
    T2CONbits.TON = 0;
}

void Timer1Delay(unsigned int delay)
{
    TMR1 = 0;
    PR1 = delay* ONE_MILLISECOND;
    T1CONbits.TON = 1;
    T1CONbits.TCKPS = 0b11;
    IFS0bits.T1IF = 0;

    while(IFS0bits.T1IF == 0);

    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;

}

void startTime() //HAVE TO USE A loop of 2 BECAUSE IM LAZY
{
    TMR1 = 0;
    PR1 = 1000*ONE_MILLISECOND;
    T1CONbits.TCKPS = 0b11;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1;
}

void startTime2()
{
    TMR1 = 0;
    PR1 = 100*ONE_MILLISECOND;
    T1CONbits.TCKPS = 0b11;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1;
}
