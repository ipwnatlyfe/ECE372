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
#include "uartComm.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define DEBOUNCE_DELAY 5 // 5 microsecond delay

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

typedef enum stateTypeEnum{
    scan, waitForPress, waitForRelease, debouncePress, debounceRelease, printKey, fail
} stateType;

volatile stateType curState = printKey;
volatile int correct = 0;
static char keys[12] = {'1', '2','3','4','5','6','7','8','9','*','0','#'};

int main(void)
{

    initKeypad();
    initUART();
    initTimer1();
    initTimer2();
    printf("Hi!\n");
    char key;
    char keyToPress;
    srand(time(NULL));

    while(1)
    {
        switch(curState){
            case printKey:
                keyToPress = keys[rand() % 12];
                printf("%c\n", keyToPress);
                curState = waitForPress;
                correct = 0;
                TMR1 = 0;
                break;
            case scan:
                key = scanKeypad();
                if(key != -1){
                    if(key == keyToPress){
                        printf("Good job!\n");
                        correct = 1;
                        TMR1 = 0;
                    }
                }
                curState = waitForRelease;
                break;
            case waitForPress:
                break;
            case debouncePress:
                delayUs(DEBOUNCE_DELAY);
                curState = scan;
                break;
            case waitForRelease:
                break;
            case debounceRelease:
                delayUs(DEBOUNCE_DELAY);
                curState = waitForPress;
                break;
            case fail:
                printf("Failed.\n");
                curState = printKey;
                break;
        }
    }

    return 0;
}

void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;
    if(COL1 == KEY_PRESSED || COL2 == KEY_PRESSED || COL3 == KEY_PRESSED) curState = debouncePress;
    else curState = debounceRelease;
}

void _ISR _T1Interrupt(void){
    IFS0bits.T1IF = 0;
    if(correct == 0) curState = fail;
    else curState = printKey;
}
