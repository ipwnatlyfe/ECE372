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
volatile int dutyLeft;
volatile int dutyRight;
volatile int newRight;
volatile int newLeft;
volatile float start = 0.0;
volatile int TIMERCOUNTER = 0;
volatile int STOPCOUNTER = 0;
volatile int RIGHTTURNS = 0;
volatile int LEFTTURNS = 0;
volatile int BLACKTOWHITE = 0;
volatile int WHITETOBLACK = 0;


int main(void)
{
initADC();
initLCD();
initCNForSW1();


TRISBbits.TRISB3 = 0;       // Pin 7 Output
TRISBbits.TRISB9 = 0;       // Pin 18 Output
TRISBbits.TRISB10 = 0;       // Pin 11 to output to go backwards
TRISBbits.TRISB8 = 0;       // Pin 17 to output to go backwards
LATBbits.LATB3 = 0;         // Set pin 7 to low to create a voltage difference
LATBbits.LATB9 = 0;         // Set pin 18 to low to create a voltage difference



currstate = IDLE;
prevstate = IDLE;
nextstate = TLEFT;
isMoving = 0;
isPressed = 0;
newRight = 0;
newLeft = 0;

while(1)
{
    if (done == 1)
    {
        moveCursorLCD(0,0);
        printVar = (adcValLeft);
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

               if(((adcValMiddle > 800) && (adcValLeft < 500) && (adcValRight < 500))|| ((adcValMiddle < 500) && (adcValLeft <500)  && (adcValRight < 500)))// Conditions for going straight
               {
                    currstate  = FORWARD;
               }
               
               else if((adcValMiddle < 500) && (adcValLeft > 800) && (adcValRight < 500))// Conditions for going left
               {
                    currstate  = ADJUSTL;
               }
               else if((adcValMiddle < 500) && (adcValLeft < 500) && (adcValRight < 500))// Conditions for going left
               {
                    currstate  = FORWARD;
               }
               else if((adcValMiddle < 500) && (adcValLeft > 800) && (adcValRight >800))
               {
                    currstate  = FORWARD;
               }
               else if((adcValMiddle < 500) && (adcValLeft < 500)  && (adcValRight > 800))// Conditions for going right
               {
                    currstate  = ADJUSTR;
               }
               
               else if(((adcValMiddle > 800) && (adcValLeft > 800)  && (adcValRight > 800)))
               {
                   currstate = STOP;
               }

               else if((adcValMiddle >800) && (adcValLeft > 800)  && (adcValRight < 500) )
               {
                  // currstate = TRIGHT;
                  

               }
               else if((adcValMiddle > 800) && (adcValLeft <500 )  && (adcValRight > 800))
               {
                       
                      currstate = KEEPLEFT;
                   
                   
               }
               else if((adcValMiddle< 500) && (adcValLeft > 800 )  && (adcValRight > 800))
               {
                   
                      
                      // currstate = KEEPLEFT;
                                  
               }
               


               prevstate = CHECKSENSORS;
               break;
           case FORWARD:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 40;
               OC2RS = 40;
               currstate = CHECKSENSORS;
               prevstate = FORWARD;
               break;

           case ADJUSTL: //Slow down Left Wheel to 40% so that we can turn left back onto the track
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 35;
               OC2RS = 0;
               currstate = CHECKSENSORS;
               prevstate = ADJUSTL;
               break;

           case ADJUSTR:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 0;
               OC2RS = 35;
               currstate = CHECKSENSORS;
               prevstate = ADJUSTR;
               break;

           case TLEFT:             
                if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 55;
               OC2RS = 0;
               

               if(LEFTTURNS < 2e)
               {
                   prevstate = KEEPLEFT;
                   currstate = KEEPLEFT;
               }
               else
               {
               currstate = CHECKSENSORS;
               LEFTTURNS = 0;
               isMoving = 0;
               start = 0;
               }
               break;
/*
               if (WHITETOBLACK < 1)
               {
                   if(adcValMiddle < 500)
                   {
                   BLACKTOWHITE++;                   
                   }

                   else if(BLACKTOWHITE > 0 && adcValMiddle > 700)
                   {
                       WHITETOBLACK++;
                   }

               }
               else
               {
                   LEFTTURNS++;
                   WHITETOBLACK = 0;
                   BLACKTOWHITE++;
               }
 * */
               currstate = CHECKSENSORS;
               prevstate = TLEFT;

              
               
               
               break;

           case TRIGHT:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               //OC1RS = 55;
               //OC2RS = 40;
               OC1RS = 0;
               OC2RS = 35;
               currstate = CHECKSENSORS;
               prevstate = TRIGHT;
               break;
               /*
              if (WHITETOBLACK < 1)
               {
                   if(adcValMiddle < 500)
                   {
                   BLACKTOWHITE++;
                   }

                   else if(BLACKTOWHITE > 0 && adcValMiddle > 700)
                   {
                       WHITETOBLACK++;
                   }

               }
               else
               {
                   RIGHTTURNS++;
                   WHITETOBLACK = 0;
                   BLACKTOWHITE++;
               }
                * */
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


           case KEEPLEFT:
                isMoving = 0;
               OC1RS = 0;
               OC2RS = 0;
               currstate = TLEFT;
               prevstate = KEEPLEFT;
               startTime();
               break;

              

           case KEEPRIGHT:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 0;
               OC2RS = 55;
               if(start == 0.0)
               {
                   startTime2();
                   start++;
               }

               if(RIGHTTURNS < 10)
               {
                   prevstate = KEEPRIGHT;
                   currstate = KEEPRIGHT;
               }
               else
               {
               currstate = CHECKSENSORS;
               //RIGHTTURNS = 0;
               isMoving = 0;
               //start = 0;
               }
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
    if(currstate == KEEPLEFT || currstate ==CHECKSENSORS)
    {
       LEFTTURNS++;
       TMR1 = 0;
    }
    else
    {
        T1CONbits.TON = 0;
    }
    if(currstate == KEEPLEFT || currstate ==CHECKSENSORS)
    {
       RIGHTTURNS++;
       TMR1 = 0;
    }
    else
    {
        T1CONbits.TON = 0;
    }

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
