#include "p24fj64ga002.h"
#include "adc.h"
#include "pwm.h"
#include "lcd.h"
#include "cn.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

typedef enum stateTypeENUM{
    IDLE, FORWARD, CHECKSENSORS, TLEFT, TRIGHT, STOP, ADJUSTL, ADJUSTR
} stateType;

volatile stateType currstate;
volatile stateType prevstate;

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
volatile int dutyLeft;
volatile int dutyRight;
volatile int newRight;
volatile int newLeft;


int main(void)
{
initADC();
initLCD();
initCNForSW1();


TRISBbits.TRISB3 = 0;       // Pin 7 Output
TRISBbits.TRISB9 = 0;       // Pin 18 Output
TRISBbits.TRISB4 = 0;       // Pin 11 to output to go backwards
TRISBbits.TRISB8 = 0;       // Pin 17 to output to go backwards
LATBbits.LATB3 = 0;         // Set pin 7 to low to create a voltage difference
LATBbits.LATB9 = 0;         // Set pin 18 to low to create a voltage difference
LATBbits.LATB4 = 0;         // Pin 11 to low to go backwards
LATBbits.LATB8 = 0;         // Pin 17 to output to go backwards


currstate = IDLE;
prevstate = IDLE;
isMoving = 0;
isPressed = 0;
newRight = 0;
newLeft = 0;

while(1)
{
    if (done == 1)
    {
        moveCursorLCD(0,0);
        printVar = (adcValMiddle);
        sprintf(str, "%.3f", printVar);
        printStringLCD(str);
        done = 0;
    }

       switch(currstate)
        {
            case IDLE:
                OC1RS = 0;
                OC2RS = 0;
                moveCursorLCD(1,0);
                printStringLCD("IDLE    ");
                isMoving = 0;
                break;

           case CHECKSENSORS:
               moveCursorLCD(1,0);
               printStringLCD("CHECKING");

               if((adcValMiddle > 700) && (adcValLeft < 200) && (adcValRight < 200))// Conditions for going straight
               {
                    currstate  = FORWARD;
               }    
               else if((adcValMiddle < 200) && (adcValLeft > 700) && (adcValRight < 200))// Conditions for going left
               {
                    currstate  = ADJUSTL;
               } 
               else if((adcValMiddle < 200) && (adcValLeft < 200)  && (adcValRight > 700))// Conditions for going right
               {
                    currstate  = ADJUSTR;
               }
               else if((adcValMiddle > 700) && (adcValLeft > 700)  && (adcValRight < 200))
               {
                   currstate = TLEFT;
               }
               else if((adcValMiddle > 700) && (adcValLeft > 700)  && (adcValRight < 200))
               {
                   currstate = TRIGHT;
               }
               else if((adcValMiddle > 700) && (adcValLeft > 700)  && (adcValRight > 700))
               {
                   currstate = STOP;
               }
               prevstate = CHECKSENSORS;
               break;
           case FORWARD:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 55;
               OC2RS = 55;
               currstate = CHECKSENSORS;
               prevstate = FORWARD;
               break;

           case ADJUSTL: //Slow down Left Wheel to 40% so that we can turn left back onto the track
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 1;
               OC2RS = 55;
               currstate = CHECKSENSORS;
               prevstate = ADJUSTL;
               break;

           case ADJUSTR:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 55;
               OC2RS = 1;
               currstate = CHECKSENSORS;
               prevstate = ADJUSTR;
               break;

           case TLEFT:             
               OC1RS = 55;
               OC2RS = 0;              
               currstate = CHECKSENSORS;
               prevstate = TLEFT;
               break;

           case TRIGHT:
               OC1RS = 0;
               OC2RS = 55;
               currstate = CHECKSENSORS;
               prevstate = TRIGHT;
               break;

           case STOP:
               isMoving = 0;
               OC1RS = 0;
               OC2RS = 0;
               currstate = CHECKSENSORS;
               prevstate = STOP;
               break;






        }

}
return 0;
}
// ******************************************************************************************* //
void _ISR _ADC1Interrupt(void){
IFS0bits.AD1IF = 0;

unsigned int *adcPtr;
adcValLeft = 0;
adcValMiddle = 0;
adcValRight = 0;
adcPtr = (unsigned int *) (&ADC1BUF0);
adcValRight = *adcPtr;
adcPtr++;
adcValLeft = *adcPtr;
adcPtr++;
adcValMiddle = *adcPtr;
adcPtr++;

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

