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
volatile float start = 0.0;
volatile int TIMERCOUNTER = 0;
volatile int RIGHTTURNS = 0;
volatile int LEFTTURNS = 0;
volatile int STOPS = 0;
volatile int TURNFROMSTOP;
volatile int hasTurned;
volatile int SWITCHED=0;


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
nextstate = IDLE;
isMoving = 0;
hasTurned = 0;
isPressed = 0;


while(1)
{
    if (done == 1)
    {
        moveCursorLCD(0,0);
        printVar = (STOPS);
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
               /******************************************************************************************
                * The next few if statements are what needs to occur once the turn is complete
                * The reason we need 2 different statements is we see if the reason we turned left is
                * because of a full stop (3 sensors covered) or because of the right turn
                * qualifications
                * *****************************************************************************************/
               else if(prevstate == TLEFT  && hasTurned == 1 && TURNFROMSTOP == 1 && SWITCHED == 1 && adcValLeft > 800) // Keeps turning LEFT until middle hits the line again
               {
                   hasTurned = 0;
                   TURNFROMSTOP = 0;
                   STOPS++;
                   SWITCHED = 0;
                   currstate = FORWARD;
               }

               else if(prevstate == TLEFT  && hasTurned == 1 && SWITCHED == 1 && adcValLeft > 800) // Keeps turning RIGHT until middle hits the line again
               {
                   hasTurned = 0;
                   LEFTTURNS++;
                   SWITCHED = 0;
                   currstate = FORWARD;
               }
               /******************************************************************************************
                * Intermediate step for turning, we also have to make sure it keeps turning even when the left sensor
                * is off the track we have to continue turning until the turn is complete. So we have a simple statement that keeps
                * the robot in turn mode until the turn is complete.
               ********************************************************************************************/
               else if(prevstate == TLEFT && adcValLeft > 800 && SWITCHED == 0)
               {
                   currstate = TLEFT;
               }
                /**************************************************************************************************
                * Conditions below are to keep turning left, meaning the turn has not yet been completed.
                * Due to the variety of turns we need to have conditions for where each sensor may be
                * Have to learn how to legimately tell when the turn is completed
                *****************************************************************************************************/
               else if(prevstate == TLEFT && adcValLeft < 500)
               {
                   SWITCHED = 1;
                   hasTurned = 1;
                   currstate = TLEFT;
               }

               /******************************************************************************************
                * The next few if statements are what needs to occur once the turn is complete
                * The reason we need 2 different statements is we see if the reason we turned right is
                * because of a full stop (3 sensors covered) or because of the right turn
                * qualifications
                * *****************************************************************************************/
                             
              else if(prevstate == TRIGHT  && hasTurned == 1 && TURNFROMSTOP == 1 && SWITCHED == 1 && adcValRight > 800) // Keeps turning Right until middle hits the line again
               {
                   hasTurned = 0;
                   TURNFROMSTOP = 0;
                   STOPS++;
                   SWITCHED = 0;
                   currstate = FORWARD;
               }

               else if(prevstate == TRIGHT  && hasTurned == 1 && SWITCHED == 1 && adcValRight > 800) // Keeps turning RIGHT until middle hits the line again
               {
                   hasTurned = 0;
                   RIGHTTURNS++;
                   SWITCHED = 0;
                   currstate = FORWARD;
               }
              /******************************************************************************************
                * Intermediate step for turning, we also have to make sure it keeps turning even when the left sensor
                * is off the track we have to continue turning until the turn is complete. So we have a simple statement that keeps
                * the robot in turn mode until the turn is complete.
               ********************************************************************************************/
               else if(prevstate == TRIGHT && adcValRight > 800 && SWITCHED == 0)
               {
                   currstate = TRIGHT;
               }

               /**************************************************************************************************
                * Conditions below are to keep turning left, meaning the turn has not yet been completed.
                * Due to the variety of turns we need to have conditions for where each sensor may be
                * Have to learn how to legimately tell when the turn is completed
                *****************************************************************************************************/
               else if(prevstate == TRIGHT && adcValRight < 500)
               {
                   SWITCHED = 1;
                   hasTurned = 1;
                   currstate = TRIGHT;
               }

               /***************************************************************************************************
                * The below conditions are marely for adjustments and not for legitimately turning and such
                *
                *
                *
                * **************************************************************************************************/

               else if((adcValMiddle < 500) && (adcValLeft > 800) && (adcValRight < 500))       // Conditions for adjusting  left
               {
                    currstate  = ADJUSTL;
               }
                            
               else if((adcValMiddle < 500) && (adcValLeft < 500)  && (adcValRight > 800))      // Conditions for adjusting right
               {
                    currstate  = ADJUSTR;
               }
               
               else if(((adcValMiddle > 800) && (adcValLeft > 800)  && (adcValRight > 800)))    // If there's black across all sensors, then stop
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

               else if((adcValMiddle >800) && (adcValLeft > 800)  && (adcValRight < 500) )      // If the left and middle sensor are over black, but the right sensor is over white
               {
                   if(LEFTTURNS < 4)
                   {
                        currstate = TLEFT;
                   }
                   else
                   {
                        currstate = FORWARD;
                   }
               }
               else if((adcValMiddle > 800) && (adcValLeft <500 )  && (adcValRight > 800))      // If the right and middle sensor are over black, but the left sensor is over white
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
               prevstate = CHECKSENSORS;
               break;
    



           case FORWARD:
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               if(SWITCHED == 1 && adcValMiddle > 800 && adcValLeft > 800 && adcValRight > 800)
               {
                    OC1RS = 40;
                    OC2RS = 30;
                    currstate = FORWARD;
                    prevstate = FORWARD;
               }
               else
               {
               OC1RS = 40;
               OC2RS = 30;
               currstate = CHECKSENSORS;
               prevstate = FORWARD;
               SWITCHED = 0;
               }
               break;

           case ADJUSTL: 
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 40;
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
               OC2RS = 40;
               currstate = CHECKSENSORS;
               prevstate = ADJUSTR;
               break;

           case TLEFT:             
               if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 35;
               OC2RS = 0;
               currstate = CHECKSENSORS;
               prevstate = TLEFT;
               break;

           case TRIGHT:
                if (isMoving == 0)
                {
                initPWM();
                isMoving = 1;
                }
               OC1RS = 0;
               OC2RS = 35;
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
