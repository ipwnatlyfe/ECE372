//File Name: lab3p2.c
// Authors: Steven Wirth, Jake Boucher, Ryan Dormond, Chang Jin
// Date: April 3, 2015
// Description: Main file that implements the FSM for part 2 of lab 3

#include "p24fj64ga002.h"
#include "adc.h"
#include "pwm.h"
#include "lcd.h"
#include "cn.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CLEAR 0
#define OUTPUT 0
#define SET 1
#define INCREMENT 1
#define MAX_VOLTAGE 3.3
#define ADC_DIVISOR 1023.0
#define ADCPTR_DIVISOR 16
#define OCxRS_VAL 55
#define ERROR 2
#define DELAY 5000

#define PORTB_RB5 PORTBbits.RB5

#define LAT_B8 LATBbits.LATB8
#define LAT_B2 LATBbits.LATB2
#define LAT_B3 LATBbits.LATB3
#define LAT_B9 LATBbits.LATB9

#define TRIS_B3 TRISBbits.TRISB3
#define TRIS_B9 TRISBbits.TRISB9
#define TRIS_B8 TRISBbits.TRISB8
#define TRIS_B2 TRISBbits.TRISB2

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

typedef enum stateTypeENUM{ //FSM STates
    IDLE, FORWARD, BACK
} stateType;

volatile stateType currstate; // indicates current state in FSM
volatile stateType prevstate; // placeholder for previous state

//volatile variable inits and strings
volatile int val = CLEAR;
volatile int done = CLEAR;
volatile int adcVal = CLEAR;
volatile float printVar = CLEAR;
static char str[15];
static char str2[15];

//volatile int inits
volatile int isPressed;
volatile int isMoving;
volatile int dutyLeft;
volatile int dutyRight;
volatile int newRight;
volatile int newLeft;

// main function that implements the FSM for lab 3 part 2
int main(void)
{
initADC();
initLCD();
initCNForSW1();


TRIS_B3 = OUTPUT; // Pin 7 Output
TRIS_B9 = OUTPUT; // Pin 18 Output
TRIS_B2 = OUTPUT; // Pin 6 to output to go backwards
TRIS_B8 = OUTPUT; // Pin 17 to output to go backwards
LAT_B3 = CLEAR; // Set pin 7 to low to create a voltage difference
LAT_B9 = CLEAR; // Set pin 18 to low to create a voltage difference
LAT_B2 = CLEAR; // Pin 6 to low to go backwards
LAT_B8 = CLEAR; // Pin 17 to output to go backwards

// state and variable inits
currstate = IDLE;
prevstate = BACK;
isMoving = CLEAR;
isPressed = CLEAR;
newRight = CLEAR;
newLeft = CLEAR;

while(1)
{
    if (done == SET) // checks to see if voltage calc variable is calculated completely
    {
        moveCursorLCD(0,0);
        printVar = (adcVal/(ADC_DIVISOR) * MAX_VOLTAGE); // calculates voltage to be displayed
        sprintf(str, "%.3f", printVar); // converts floaqt to string
        printStringLCD(str); // prints string
        done = CLEAR; // resets the check calculation flag
    }

       switch(currstate)
        {
            case IDLE:
                OC1RS = CLEAR; // sets voltage to 0 on 1 motor
                OC2RS = CLEAR; // sets voltage to 0 on the other motor
                moveCursorLCD(1,0);
                printStringLCD("IDLE   ");
                isMoving = CLEAR; // flag is set to 0
                break;

            case FORWARD:
                if (isMoving == CLEAR) // checks to see flag was not incremented
                {
                initPWM();
                isMoving = SET; // incrments flag

                }
                
                moveCursorLCD(1,0);
                printStringLCD("FORWARD");

                newRight = (adcVal/(ADC_DIVISOR) * OCxRS_VAL); // calculates updated right voltage
                newLeft = (1-(adcVal/(ADC_DIVISOR))) * OCxRS_VAL; // calculates updates left voltage
                if(abs(newRight - newLeft)<ERROR) // checks to see if the difference is within an error amount
                {
                    OC1RS = OCxRS_VAL;
                    OC2RS = OCxRS_VAL;
                }
                else // sets the motor to new calculated voltages for each side
                {
                    OC1RS = newRight;
                    OC2RS = newLeft;
                }
                break;

            case BACK:

                if (isMoving == CLEAR) // checks if flag wasn't incremented
                {
                initBackPWM();
                isMoving = SET; //increments flag
                }
                
                moveCursorLCD(1,0);
                printStringLCD("BACK  ");


                newRight = (adcVal/(ADC_DIVISOR) * OCxRS_VAL); // calculates updated right voltage
                newLeft = (INCREMENT-(adcVal/(ADC_DIVISOR))) * OCxRS_VAL; // calculates updates left voltage
                if(abs(newRight - newLeft)<ERROR) // checks to see if the difference is within an error amount
                {
                    OC1RS = OCxRS_VAL;
                    OC2RS = OCxRS_VAL;
                }
                else // sets the motor to new calculated voltages for each side
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

// analog to digital conversion interrupt to calculate variable used in voltage calculation
void _ISR _ADC1Interrupt(void){
IFS0bits.AD1IF = CLEAR; // put the flag down


int i = CLEAR; // init loop var
unsigned int *adcPtr;
adcVal = CLEAR; // init var for voltage calc
adcPtr = (unsigned int *) (&ADC1BUF0); // unassigned int cast a ptr to the val at ADC1BUF0 address
for(i = CLEAR; i < ADCPTR_DIVISOR; i++){ // loop to calculate var adcVal used in the voltage calculation
adcVal = adcVal + *adcPtr/ADCPTR_DIVISOR;
adcPtr++;
}

done = SET; // verifies calculation is complete

}


// change notification interrupt changes the state of the FSM upon each button press
void _ISR _CNInterrupt(void)
{
    IFS1bits.CNIF = CLEAR;
    delayUs(DELAY);

    if (PORTB_RB5 == PRESSED && isPressed == CLEAR) // checks to see if button is pressed
    {
        isPressed = SET; // flags as pressed
    }
    else if (PORTB_RB5 == RELEASED && isPressed == SET) // checks to see if Button is released
    {
        isPressed = CLEAR; // resets flag for button press

        if(currstate == IDLE && prevstate == FORWARD) // chacks state then changes it
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