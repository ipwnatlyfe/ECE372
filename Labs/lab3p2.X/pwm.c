#include "p24FJ64GA002.h"
#include "pwm.h"
void initPWM(){
/*
TRISBbits.TRISB3 = 0; // Pin 7 Output
TRISBbits.TRISB9 = 0; // Pin 18 Output
LATBbits.LATB3 = 0; // Set pin 7 to low to create a voltage difference
LATBbits.LATB9 = 0; // Set pin 18 to low to create a voltage difference
*/
RPOR1bits.RP3R = 0; // map OC1 to pin 7
RPOR4bits.RP9R = 0; // map OC2 to pin 18

LATBbits.LATB3 = 0; // Set pin 7 to low to create a voltage difference
LATBbits.LATB9 = 0; // Set pin 18 to low to create a voltage difference

OC1CONbits.OCM = 0;
OC2CONbits.OCM = 0;
T3CONbits.TCKPS = 3;
TMR3 = 0;
PR3 = 56;
OC1CONbits.OCTSEL = 1; // using timer 3
OC1CONbits.OCM = 6;
RPOR1bits.RP3R = 18; // map OC1 to pin 6
OC1R = 0; //very important
OC1RS = 55;

TMR3 = 0;
PR3 = 56;
OC2CONbits.OCTSEL = 1; // using timer 3
OC2CONbits.OCM = 6;
RPOR4bits.RP8R = 19; // map OC2 to pin 17
OC2R = 0; //very important
OC2RS = 55;
T3CONbits.TON = 1;

}

void initBackPWM(){
/*
TRISBbits.TRISB2 = 0; // Pin 6 to output to go backwards
TRISBbits.TRISB8 = 0; // Pin 17 to output to go backwards
LATBbits.LATB2 = 0; // Pin 6 to low to go backwards
LATBbits.LATB8 = 0; // Pin 17 to output to go backwards
*/
RPOR1bits.RP2R = 0; // map OC1 to pin 6
RPOR4bits.RP8R = 0; // map OC2 to pin 17

LATBbits.LATB2 = 0; // Pin 6 to low to go backwards
LATBbits.LATB8 = 0; // Pin 17 to output to go backwards

OC1CONbits.OCM = 0;
OC2CONbits.OCM = 0;
T3CONbits.TCKPS = 3;
TMR3 = 0;
PR3 = 56;
OC1CONbits.OCTSEL = 1; // using timer 3
OC1CONbits.OCM = 6;
RPOR1bits.RP3R = 18; // map OC1 to pin 7
OC1R = 0; //very important
OC1RS = 55;


OC2CONbits.OCTSEL = 1; // using timer 3
OC2CONbits.OCM = 6;
RPOR4bits.RP9R = 19; // map OC2 to pin 18
OC2R = 0; //very important
OC2RS = 55;
T3CONbits.TON = 1;

}
