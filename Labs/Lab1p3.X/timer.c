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

void startTime()
{
    TMR1 = 0;
    PR1 = ONE_MILLISECOND;
    T1CONbits.TCKPS = 0b11;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1;
}



char* getTimeString(unsigned int c)
{
    int min=0;
    int sec =0;
    int ms =0;
    static char s[9];




    char min1 = '0';
    char min2 = '0';
    char min3 = '0';
    char sec1 = '0';
    char sec2 = '0';
    char sec3 = '0';
    char ms1 = '0';
    char ms2 = '0';
   

    
    min = c / MIN_TO_MS;
    sec = (c - (min * MIN_TO_MS)) / SEC_TO_MS;
    ms = (c - (min * MIN_TO_MS) - (sec * SEC_TO_MS));


    
    min2 = (min/10)+'0';
    min3 = (min%10)+'0';

    
    sec2 = (sec/10)+'0';
    sec3 = (sec%10)+'0';

    ms1 = (ms/100) +'0';
    ms2 = ((ms%100)/10) +'0';

    
    s[0] = min2;
    s[1] = min3;
    s[2] = ':';
    s[3] = sec2;
    s[4] = sec3;
    s[5] = ':';
    s[6] = ms1;
    s[7] = ms2;
    s[8] = '\0';

    return s;
}