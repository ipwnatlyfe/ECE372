#include "p24FJ64GA002.h"
#include "adc.h"

#define SET 1
#define RESET 0
#define CLOCK_PERIOD 10
#define AUTO_CONVERT 7
#define SAMPLE_SEQUENCE 0b1111


//This is the initialization function for initializing the Analog to digital conversion
void initADC(){
AD1PCFGbits.PCFG0 = RESET; //Set pin 25 to analog
AD1CON1bits.FORM = RESET; // Data Output set to interger
AD1CON1bits.SSRC = AUTO_CONVERT; // Conversion trigger set to auto convert
AD1CON1bits.ASAM = SET; // Sampling begins after the last one is finished
AD1CON2bits.VCFG = RESET; // voltage reference configuration bits aVdd <--> AVss
AD1CON2bits.CSCNA = RESET; // use the channel selected by the CH0SA bits as the MUX A input
AD1CON2bits.SMPI = SAMPLE_SEQUENCE; // Convert sequence Interrupts at the completion of conversion for each sample/convert sequence
AD1CON2bits.ALTS = RESET; // alternate input sample mode select bit always uses mux A
AD1CON3bits.ADRC = RESET; // uses system clock
AD1CON3bits.SAMC = SET; // auto sample 1 TAD
AD1CON3bits.ADCS = CLOCK_PERIOD; // A/D conversion clock period select bits (set to 10)
AD1CHS = RESET; //Configure input channels,
AD1CSSL = RESET; //Channel scanning is disabled anyway.
AD1CON1bits.ADON = SET; // Turn on A/D
IEC0bits.AD1IE = SET; //Enable the interrupt
IFS0bits.AD1IF = RESET; //Put the flag down
}

// This function was used in testing purposes when we thought sprintf wouldnt work
char* getVoltString(int c)
{

    static char s[4]; // init string to hold voltsage



    s[0] = c / 10; // find val to left of decimal
    s[1] = '.'; // store decimal in the string
    s[2] = c % 10; // finds val to the right of the decimal
    s[3] = '\0'; //  null character

    return s; // return string
}

