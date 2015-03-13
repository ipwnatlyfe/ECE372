#include "p24FJ64GA002.h"
#include "keypad.h"
#include "timer.h"


/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){

    TRISAbits.TRISA0 = OUTPUT; // pin 9 ROW1
    TRISAbits.TRISA1 = OUTPUT; // pin 10 ROW2
    TRISBbits.TRISB2 = OUTPUT; // pin 14 ROW3
    TRISBbits.TRISB9 = OUTPUT; // pin 17 ROW4

    TRISBbits.TRISB8 = INPUT; // pin 18 COL1
    TRISBbits.TRISB10 = INPUT; // pin 21 COL2
    TRISBbits.TRISB11 = INPUT; // pin 22 COL3

    ODCAbits.ODA0 = 1; // pin 9
    ODCAbits.ODA1 = 1; // pin 10
    ODCBbits.ODB2 = 1; // pin 14
    ODCBbits.ODB9 = 1; // pin 16

    AD1PCFGbits.PCFG0 = 1;
    AD1PCFGbits.PCFG1 = 1;
    AD1PCFGbits.PCFG4 = 1;

    IEC1bits.CNIE = 1;

    IFS1bits.CNIF = 0;


    CNPU2bits.CN22PUE = 1;
    CNEN2bits.CN22IE = 1;

    CNPU1bits.CN15PUE = 1;
    CNEN1bits.CN15IE = 1;

    CNPU2bits.CN16PUE = 1;
    CNEN2bits.CN16IE = 1;





}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 */
char scanKeypad(void){


    char key = -1;

    IEC1bits.CNIE = 0;

    ROW1 = ON; // pin 9
    ROW2 = OFF; // pin 10
    ROW3 = OFF; // pin 14
    ROW4 = OFF; // pin 16
    delayUs(50);
    if(COL1 == ON)
    {
        key = '1';
    }
    else if(COL2 == ON)
    {
        key = '2';
    }
    else if(COL3 == ON)
    {
        key = '3';
    }

    ROW1 = OFF; // pin 9
    ROW2 = ON; // pin 10
    ROW3 = OFF; // pin 14
    ROW4 = OFF; // pin 16
    delayUs(50);
   if(COL1 == ON)
    {
        key = '4';
    }
    else if(COL2 == ON)
    {
        key = '5';
    }
    else if(COL3 == ON)
    {
        key = '6';
    }

    ROW1 = OFF; // pin 9
    ROW2 = OFF; // pin 10
    ROW3 = ON; // pin 14
    ROW4 = OFF; // pin 16
    delayUs(50);
     if(COL1 == ON)
    {
        key = '7';
    }
    else if(COL2 == ON)
    {
        key = '8';
    }
    else if(COL3 == ON)
    {
        key = '9';
    }

    ROW1 = OFF; // pin 9
    ROW2 = OFF; // pin 10
    ROW3 = OFF; // pin 14
    ROW4 = ON; // pin 16
    delayUs(50);
    if(COL1 == ON)
    {
        key = '*';
    }
    else if(COL2 == ON)
    {
        key = '0';
    }
    else if(COL3 == ON)
    {
        key = '#';
    }

    ROW1 = ON; // pin 9
    ROW2 = ON; // pin 10
    ROW3 = ON; // pin 14
    ROW4 = ON; // pin 16

    IEC1bits.CNIE = 1;
    IFS1bits.CNIF = 0;
    return key;
}
