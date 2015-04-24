#include "p24FJ64GA002.h"
#include "adc.h"

#define SET 1
#define RESET 0
#define CLOCK_PERIOD 10
#define AUTO_CONVERT 7
#define SAMPLE_SEQUENCE 0b0010


//This is the initialization function for initializing the Analog to digital conversion
void initADC(){
AD1PCFGbits.PCFG1 = RESET;          //Set pin 3 to analog
AD1PCFGbits.PCFG0 = RESET;          //Set pin 2 to analog
AD1PCFGbits.PCFG4 = RESET;          //Set pin 6 to analog

AD1CON1bits.FORM = RESET;           // Data Output set to interger
AD1CON1bits.SSRC = AUTO_CONVERT;    // Conversion trigger set to auto convert
AD1CON1bits.ASAM = SET;             // Sampling begins after the last one is finished
AD1CON2bits.VCFG = RESET;           // voltage reference configuration bits aVdd <--> AVss
AD1CON2bits.CSCNA = 1;              // uses the mux a channels to be the input
AD1CON2bits.SMPI = SAMPLE_SEQUENCE; // Convert sequence Interrupts at the completion of conversion for each sample/convert sequence
AD1CON2bits.ALTS = RESET;           // alternate input sample mode select bit always uses mux A
AD1CON3bits.ADRC = RESET;           // uses system clock
AD1CON3bits.SAMC = SET;             // auto sample 1 TAD
AD1CON3bits.ADCS = CLOCK_PERIOD;    // A/D conversion clock period select bits (set to 10)
AD1CHS = RESET;                     //Configure input channels,
AD1CSSL = 19;                       //Set pins 3,6,and 2 to be scanned. Channels 0,1,4
AD1CON1bits.ADON = SET;             // Turn on A/D
IEC0bits.AD1IE = SET;               //Enable the interrupt
IFS0bits.AD1IF = RESET;             //Put the flag down
}



