#include "p24FJ64GA002.h"
#include "keypad.h"
#include "timer.h"

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){
    TRISBbits.TRISB2 = 0; // pin 25
    TRISBbits.TRISB3 = 0; // pin 24
    TRISAbits.TRISA0 = 0; // pin 23
    TRISAbits.TRISA1 = 0; // pin 26

    TRISBbits.TRISB7 = 1; // pin 18
    TRISBbits.TRISB10 = 1; // pin 21
    TRISBbits.TRISB11 = 1; // pin 22

    ODCAbits.ODA0 = 1; // pin 23
    ODCAbits.ODA1 = 1; // pin 26
    ODCBbits.ODB2 = 1; // pin 25
    ODCBbits.ODB3 = 1; // pin 24

    IFS1bits.CNIF = 0;

    CNEN2bits.CN21IE = 1;
    CNPU2bits.CN21PUE = 1;

    CNEN1bits.CN15IE = 1;
    CNPU1bits.CN15PUE = 1;

    CNEN2bits.CN16IE = 1;
    CNPU2bits.CN16PUE = 1;

    IEC1bits.CNIE = 1;
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 */
char scanKeypad(void){
    char key = -1;

    ODCAbits.ODA0 = 1; // pin 23
    ODCAbits.ODA1 = 0; // pin 26
    ODCBbits.ODB2 = 0; // pin 25
    ODCBbits.ODB3 = 0; // pin 24

    if(LATBbits.LATB7 == 0)
    {
        return 0x31;
    }
    else if(LATBbits.LATB10 == 0)
    {
        return 0x32;
    }
    else if(LATBbits.LATB11 == 0)
    {
        return 0x33;
    }

    ODCAbits.ODA0 = 0; // pin 23
    ODCAbits.ODA1 = 1; // pin 26
    ODCBbits.ODB2 = 0; // pin 25
    ODCBbits.ODB3 = 0; // pin 24

    if(LATBbits.LATB7 == 0)
    {
        return 0x34;
    }
    else if(LATBbits.LATB10 == 0)
    {
        return 0x35;
    }
    else if(LATBbits.LATB11 == 0)
    {
        return 0x36;
    }

    ODCAbits.ODA0 = 0; // pin 23
    ODCAbits.ODA1 = 0; // pin 26
    ODCBbits.ODB2 = 1; // pin 25
    ODCBbits.ODB3 = 0; // pin 24

    if(LATBbits.LATB7 == 0)
    {
        return 0x37;
    }
    else if(LATBbits.LATB10 == 0)
    {
        return 0x38;
    }
    else if(LATBbits.LATB11 == 0)
    {
        return 0x39;
    }

    ODCAbits.ODA0 = 0; // pin 23
    ODCAbits.ODA1 = 0; // pin 26
    ODCBbits.ODB2 = 0; // pin 25
    ODCBbits.ODB3 = 1; // pin 24

    if(LATBbits.LATB7 == 0)
    {
        return 0x3A;
    }
    else if(LATBbits.LATB10 == 0)
    {
        return 0x3B;
    }
    else if(LATBbits.LATB11 == 0)
    {
        return 0x3C;
    }

    return key;
}
