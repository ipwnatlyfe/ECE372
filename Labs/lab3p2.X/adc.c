#include "p24FJ64GA002.h"
#include "adc.h"
void initADC(){
TRISAbits.TRISA0 = 1;


AD1PCFGbits.PCFG0 = 0; //Pin for corresponding analog channel is in Analog mode AN0
AD1CON1bits.FORM = 0; // Data Output set to integer.
AD1CON1bits.SSRC = 7; // Conversion trigger set to auto-convert
AD1CON1bits.ASAM = 1; // Sampling begins after the last one is finished
AD1CON2bits.VCFG = 0; // Voltage reference configuration bits AVdd <--> AVss
AD1CON2bits.CSCNA = 0; // Use the channel selected by the CH0SA bits as the MUX A input
AD1CON2bits.SMPI = 0b0000; // Convert sequence Interrupts at the completion of conversion for each sample/convert sequence
AD1CON2bits.ALTS = 0; // Alternate Input Sample Mode Select bit Always uses Mux A
AD1CON3bits.ADRC = 0; // Uses system clock
AD1CON3bits.SAMC = 1; // Auto Sample 1 TAD
AD1CON3bits.ADCS = 10; // A/D Conversion Clock Period Select bits(set to 10)
AD1CHS = 0; //Configure input channels,
AD1CHSbits.CH0SB = 0; //Amplifier Positive Input Select for MUX B Multiplexer Setting bits (corresponds to AN0 or pin 2)
AD1CSSL = 0; //Channel scanning is disabled anyway.
AD1CON1bits.ADON = 1; // Turn on A/D
IEC0bits.AD1IE = 1; //Enable the interrupt
IFS0bits.AD1IF = 0; //Put the flag down
}

char* getVoltString(int c)
{

    static char s[3];



    s[0] = c / 100;
    s[1] = c %10;
    s[2] = '\0';
    

    return s;
}

