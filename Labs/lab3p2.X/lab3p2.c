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
    IDLE, FORWARD, BACK
} stateType;

volatile stateType currstate;
volatile stateType prevstate;

volatile int val = 0;
volatile int done = 0;
volatile int adcVal = 0;
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


TRISBbits.TRISB3 = 0; // Pin 7 Output
TRISBbits.TRISB9 = 0; // Pin 18 Output
TRISBbits.TRISB2 = 0; // Pin 6 to output to go backwards
TRISBbits.TRISB8 = 0; // Pin 17 to output to go backwards
LATBbits.LATB3 = 0; // Set pin 7 to low to create a voltage difference
LATBbits.LATB9 = 0; // Set pin 18 to low to create a voltage difference
LATBbits.LATB2 = 0; // Pin 6 to low to go backwards
LATBbits.LATB8 = 0; // Pin 17 to output to go backwards


currstate = IDLE;
prevstate = BACK;
isMoving = 0;
isPressed = 0;
newRight = 0;
newLeft = 0;

while(1)
{
    if (done == 1)
    {
        moveCursorLCD(0,0);
        printVar = (adcVal/(1023.0) * 3.3);
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
                printStringLCD("IDLE   ");
                isMoving = 0;
                break;

            case FORWARD:                
                if (isMoving == 0)
                {              
                initPWM();
                isMoving = 1;

                }
                /*
                else
                {
                    RPOR1bits.RP3R = 0; // map OC1 to pin 7
                    RPOR4bits.RP9R = 0; // map OC2 to pin 18

                    LATBbits.LATB3 = 0; // Set pin 7 to low to create a voltage difference
                    LATBbits.LATB9 = 0; // Set pin 18 to low to create a voltage difference
                    
                    RPOR1bits.RP2R = 18; // map OC1 to pin 6
                    RPOR4bits.RP8R = 19; // map OC2 to pin 17
                    
                }
               */
                moveCursorLCD(1,0);
                printStringLCD("FORWARD");

                newRight = (adcVal/(1023.0) * 55);
                newLeft = (1-(adcVal/(1023.0))) * 55;
                if(abs(newRight - newLeft)<2)
                {
                    OC1RS = 55;
                    OC2RS = 55;
                }
                else
                {
                    OC1RS = newRight;
                    OC2RS = newLeft;
                }
                break;

            case BACK:

                if (isMoving == 0)
                {
                initBackPWM();
                isMoving = 1;
                }
                /*
                else
                {
                    RPOR1bits.RP2R = 0; // map OC1 to pin 6
                    RPOR4bits.RP8R = 0; // map OC2 to pin 17

                    LATBbits.LATB2 = 0; // Pin 6 to low to go backwards
                    LATBbits.LATB8 = 0; // Pin 17 to output to go backwards
                    
                    RPOR1bits.RP3R = 18; // map OC1 to pin 7
                    RPOR4bits.RP9R = 19; // map OC2 to pin 18
                    OC1RS = 55;
                }
                 * */
                moveCursorLCD(1,0);
                printStringLCD("BACK  ");


                newRight = (adcVal/(1023.0) * 55);
                newLeft = (1-(adcVal/(1023.0))) * 55;
                if(abs(newRight - newLeft)<2)
                {
                    OC1RS = 55;
                    OC2RS = 55;
                }
                else
                {
                    OC1RS = newRight;
                    OC2RS = newLeft;
                }

                break;
                 

        }

}
return 0;
}
// ******************************************************************************************* //
void _ISR _ADC1Interrupt(void){
IFS0bits.AD1IF = 0;


int i = 0;
unsigned int *adcPtr;
adcVal = 0;
adcPtr = (unsigned int *) (&ADC1BUF0);
for(i = 0; i < 16; i++){
adcVal = adcVal + *adcPtr/16;
adcPtr++;
}

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

        if(currstate == IDLE && prevstate == FORWARD)
        {
            currstate = BACK;
            prevstate = IDLE;
        }

        else if(currstate == IDLE && prevstate == BACK)
        {
            currstate = FORWARD;
            prevstate = IDLE;
        }

        else if(currstate == FORWARD)
        {
            currstate = IDLE;
            prevstate = FORWARD;

        }

        else if(currstate == BACK)
        {
            currstate = IDLE;
            prevstate = BACK;

        }
        
    }
}
