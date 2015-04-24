#include "p24fj64ga002.h"
#include "adc.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )


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

while(1)
{
    if (done == 1)
    {
        moveCursorLCD(0,0);
        printVar = adcVal;
        //printVar = (adcVal/(1023.0) * 5.0);
        sprintf(str, "%.3f", printVar);
        printStringLCD(str);
        done = 0;
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
    /*
IFS0bits.AD1IF = 0;

unsigned int *adcPtr;
adcVal = 0;
adcPtr = (unsigned int *) (&ADC1BUF0);

adcVal = *adcPtr;

done = 1;
*/
}


