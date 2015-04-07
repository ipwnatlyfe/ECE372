/******************************************************
 Filename: lab3p1.c
 Authors: Steven Wirth, Jake Boucher, Ryan Dormond, Chang Jin
 Date: 4/3/15
 Description: This is the main file containing the FSM implementation for part 1 of lab 3

 *****************************************************************************************/

#include "p24fj64ga002.h"
#include "adc.h"
#include "pwm.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>

#define RESET 0
#define INCREMENTED 1
#define MAX_VOLTAGE 3.3
#define ADC_DIVISOR 1023.0
#define FLAG_DOWN IFS0bits.AD1IF
#define ADCPTR_DIVISOR 16

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

volatile int val = RESET; // temporary variable used in testing
volatile int done = RESET; // var to check if the adcval calculation was completed
volatile int adcVal = RESET; // value to be calculated in the interrupt to use in voltage calculation
volatile float printVar = RESET; // float var to hold voltage to be printed
static char str[15]; // string to be printed

// main function for implementation of the FSM for part 1 of lab 3
int main(void)
{
initADC(); //initialize analog to digital conversion
initLCD(); // initialize lcd
//initPWM();
initBackPWM(); // initializes backwards wheel motion
//initTrial();
while(1)
{
    if (done == INCREMENTED) // see if adcVal has been calculated
    {
        moveCursorLCD(0,0); // move cursor to top left corner
        printVar = (adcVal/(ADC_DIVISOR) * MAX_VOLTAGE); // calculates voltage to be printed on lcd
        sprintf(str, "%.3f", printVar); // converts the float value to a string to be printed on the lcd
        printStringLCD(str); // prints voltage to lcd
        done = RESET; // reset calculation verification
    }

}
return 0; // return statement
}
// ******************************************************************************************* //

// ISR for the ADC Interrupt, used to calculate variable to be used in voltage calculation
void _ISR _ADC1Interrupt(void){
FLAG_DOWN = RESET; // put the flag down


int i = RESET; // init i
unsigned int *adcPtr; //init ptr
adcVal = RESET; // init adcVal
adcPtr = (unsigned int *) (&ADC1BUF0); // unassigned int ptr cast val at ADC1BUF0 address
for(i = RESET; i < ADCPTR_DIVISOR; i++){ // loop to calculate variable used in voltage calculation
adcVal = adcVal + *adcPtr/ADCPTR_DIVISOR;
adcPtr++;
}

done = INCREMENTED; // verify calculation completed

}

