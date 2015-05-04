#include "p24fj64ga002.h"
#include "leds.h"

void initLEDs(){

    //TODO: Initialize the appropriate pins to work with the LEDs
    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB15 = 0;

    ODCBbits.ODB13 = 1;
    ODCBbits.ODB14 = 1;
    ODCBbits.ODB15 = 1;
}
