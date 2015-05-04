#include "p24fj64ga002.h"
#include "adc.h"
#include "pwm.h"
#include "cn.h"
#include "leds.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DARK 800
#define LIGHT 100



_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

typedef enum stateTypeENUM{
    IDLE, FORWARD, CHECKSENSORS, TLEFT, TRIGHT, STOP, ADJUSTL, ADJUSTR, YOUSPINMERIGHTROUND, KEEPLEFT, KEEPRIGHT
} stateType;

volatile stateType currstate;
volatile stateType prevstate;
volatile stateType nextstate;

volatile int val = 0;
volatile int done = 0;
volatile int adcValLeft = 0;
volatile int adcValMiddle = 0;
volatile int adcValRight = 0;
volatile float printVar = 0;
static char str[15];
static char str2[15];

volatile int isPressed;
volatile int isMoving;
volatile float start = 0.0;
volatile int TIMERCOUNTER = 0;
volatile int RIGHTTURNS = 0;
volatile int LEFTTURNS = 0;
volatile int STOPS = 0;
volatile int TURNFROMSTOP;
volatile int hasTurned;
volatile int SWITCHED=0;
volatile int TESTCOUNTER=0;

char c;


int main(void)
{
initADC();
initLCD();
initCNForSW1();
initI2C();


TRISBbits.TRISB3 = 0;       // Pin 7 Output
TRISBbits.TRISB0 = 0;       // Pin 14 Output
TRISBbits.TRISB10 = 0;       // Pin 11 to output to go backwards
TRISBbits.TRISB11 = 0;       // Pin 22 to output to go backwards
LATBbits.LATB3 = 0;         // Set pin 7 to low to create a voltage difference
LATBbits.LATB0 = 0;         // Set pin 14 to low to create a voltage difference

currstate = IDLE;
prevstate = IDLE;
nextstate = IDLE;
isMoving = 0;
hasTurned = 0;
isPressed = 0;

TRISBbits.TRISB15 = 0;
LATBbits.LATB15 = 1;

while(1)
{

    delayUs(1000);
       switch(currstate)
        {
            case IDLE:
                OC1RS = 0;
                OC2RS = 0;
                isMoving = 0;
                break;

           case FORWARD:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 30;
               OC2RS = 35;
               currstate = CHECKSENSORS;
               prevstate = FORWARD;
               break;

           case STOP:
               isMoving = 0;
               OC1RS = 0;
               OC2RS = 0;
               prevstate = STOP;
               break;
        }

}
return 0;
}
// ******************************************************************************************* //
void _ISR _ADC1Interrupt(void){
IFS0bits.AD1IF = 0;



done = 1;

}

void _ISR _CNInterrupt(void)
{
    IFS1bits.CNIF = 0;
    delayUs(5000);

    if (PORTBbits.RB5 == PRESSED && isPressed == 0)
    {
        isPressed = 1;
    }
    else if (PORTBbits.RB5 == RELEASED && isPressed == 1)
    {
        isPressed = 0;

        if(currstate != IDLE)
        {
            currstate = IDLE;
        }
        else
        {
        currstate = CHECKSENSORS;
        }
    }
}

void __attribute__((__interrupt__, __shadow__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;

    if(currstate == YOUSPINMERIGHTROUND)
    {
       TIMERCOUNTER++;
       TMR1 = 0;
    }
    else
    {
        T1CONbits.TON = 0;
    }


}

void _ISR _MI2C1Interrupt(void){
    IFS1bits.MI2C1IF = 0;
}