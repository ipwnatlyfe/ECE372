#include "p24fj64ga002.h"
#include "adc.h"
#include "pwm.h"
#include "cn.h"
#include "leds.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DARK 800
#define LIGHT 500



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
volatile int STATECOUNTER = 0;


int main(void)
{
initADC();
initLEDs();
//initLCD();
initCNForSW1();


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


while(1)
{
   
    if (done == 1)
    {
        done = 0;

        if(adcValLeft > DARK)
        {
            LATBbits.LATB13 = 0;
        }
        else
        {
            LATBbits.LATB13 = 1;
        }
        if(adcValMiddle > DARK)
        {
            LATBbits.LATB14 = 0;
        }
        else
        {
            LATBbits.LATB14 = 1;
        }
        if(adcValRight > DARK)
        {
            LATBbits.LATB15= 0;
        }
        else
        {
            LATBbits.LATB15 = 1;
        }

    }

       switch(currstate)
        {
            case IDLE:
                OC1RS = 0;
                OC2RS = 0;
                isMoving = 0;
                break;

           case CHECKSENSORS:

               if((adcValMiddle > DARK) && (adcValLeft < LIGHT) && (adcValRight < LIGHT))// Conditions for going straight
               {
                    currstate  = FORWARD;
               }

               else if((adcValMiddle < LIGHT && adcValLeft < LIGHT && adcValRight < LIGHT) || (adcValLeft > DARK && adcValMiddle < LIGHT && adcValRight > DARK))
               {
                   currstate = prevstate;
               }
              
               else if(prevstate == TLEFT)
               {
                   if(adcValLeft < LIGHT && adcValRight<LIGHT && adcValMiddle > DARK)
                   {
                       currstate = FORWARD;
                       LEFTTURNS++;
                   }
                   else
                   {
                       currstate = TLEFT;
                   }
               }
               else if(prevstate == TRIGHT)
               {
                   if(adcValLeft < LIGHT && adcValRight <LIGHT && adcValMiddle > DARK)
                   {
                       currstate = FORWARD;
                       RIGHTTURNS++;
                   }
                   
                   else
                   {
                       currstate = TRIGHT;
                   }
               }


                else if((adcValMiddle >DARK) && (adcValLeft > DARK)  && (adcValRight < LIGHT) )      // If the left and middle sensor are over black, but the right sensor is over white
               {
                   if(LEFTTURNS < 3)
                   {
                        currstate = TLEFT;
                   }
                   else
                   {
                        currstate = FORWARD;
                   }
               }
               else if((adcValMiddle > DARK) && (adcValLeft <LIGHT )  && (adcValRight > DARK))      // If the right and middle sensor are over black, but the left sensor is over white
               {
                   if(RIGHTTURNS < 4)
                   {
                        currstate = TRIGHT;
                   }
                   else
                   {
                        currstate = FORWARD;
                   }
               }

               else if((adcValMiddle < LIGHT) && (adcValLeft > DARK) && (adcValRight < LIGHT))       // Conditions for adjusting  left
               {
                    currstate  = ADJUSTL;
               }
                            
               else if((adcValMiddle < LIGHT) && (adcValLeft < LIGHT)  && (adcValRight > DARK))      // Conditions for adjusting right
               {
                    currstate  = ADJUSTR;
               }
               
               else if(((adcValMiddle > DARK) && (adcValLeft > DARK)  && (adcValRight > DARK)))    // If there's black across all sensors, then stop
               {
                   TURNFROMSTOP = 1;
                   if(STOPS == 3)
                   {
                        currstate = STOP;
                   }

                   else if(STOPS == 4 || STOPS == 5)
                   {
                       currstate = TLEFT;
                   }
                   else
                   {
                        currstate = TRIGHT;
                     
                   }


               }

                           
               prevstate = CHECKSENSORS;
               break;
    



           case FORWARD:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               
               OC1RS = 35;
               OC2RS = 45;
               currstate = CHECKSENSORS;
               prevstate = FORWARD;                             
               break;

           case ADJUSTL: 
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 00;
               OC2RS = 45;
               currstate = CHECKSENSORS;
               prevstate = ADJUSTL;
               break;

           case ADJUSTR:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 45;
               OC2RS = 00;
               currstate = CHECKSENSORS;
               prevstate = ADJUSTR;
               break;

           case TLEFT:             
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 00;
               OC2RS = 45;
               currstate = CHECKSENSORS;
               prevstate = TLEFT;
               break;

           case TRIGHT:
                if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
                else if((adcValMiddle > DARK) && (adcValLeft > DARK)  && (adcValRight > DARK) && prevstate != TRIGHT)
                   {
                       currstate = TRIGHT;
                       STOPS++;
                   }
               OC1RS = 45;
               OC2RS = 00;
               currstate = CHECKSENSORS;
               prevstate = TRIGHT;
               break;

           case STOP:
               isMoving = 0;
               OC1RS = 0;
               OC2RS = 0;              
               currstate = YOUSPINMERIGHTROUND;
               prevstate = STOP;
               startTime();            
               break;


           case YOUSPINMERIGHTROUND:
               initPWMSpin();
               OC1RS = 50;
               OC2RS = 50;
               
               if(TIMERCOUNTER < 1)
               {
                   prevstate = YOUSPINMERIGHTROUND;
                   currstate = YOUSPINMERIGHTROUND;
               }
               else
               {
               currstate = CHECKSENSORS;
               TIMERCOUNTER = 0;
               isMoving = 0;
               }
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
