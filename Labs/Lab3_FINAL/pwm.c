#include "p24FJ64GA002.h"
#include "pwm.h"

#define TRIS_B3 TRISBbits.TRISB3
#define TRIS_B9 TRISBbits.TRISB9
#define TRIS_B8 TRISBbits.TRISB8
#define TRIS_B2 TRISBbits.TRISB2

#define LAT_B8 LATBbits.LATB8
#define LAT_B2 LATBbits.LATB2
#define LAT_B3 LATBbits.LATB3
#define LAT_B9 LATBbits.LATB9

#define RPOR_RP2R RPOR1bits.RP2R
#define RPOR_RP3R RPOR1bits.RP3R
#define RPOR_RP8R RPOR4bits.RP8R
#define RPOR_RP9R RPOR4bits.RP9R

#define RESET 0
#define ON 1
#define PR3_VAL 56
#define T3_PR 3
#define OCxRS_VAL 55
#define OCxCON_VAL 6
#define OC1 18
#define OC2 19


//this function initializes the pulse width modulation for forward movement
void initPWM(){

RPOR_RP3R = RESET; // map OC1 to pin 7
RPOR_RP9R = RESET; // map OC2 to pin 18

LAT_B3 = RESET; // Set pin 7 to low to create a voltage difference
LAT_B9 = RESET; // Set pin 18 to low to create a voltage difference

OC1CONbits.OCM = RESET;
OC2CONbits.OCM = RESET;
T3CONbits.TCKPS = T3_PR;
TMR3 = RESET;
PR3 = PR3_VAL;
OC1CONbits.OCTSEL = ON; // using timer 3
OC1CONbits.OCM = OCxCON_VAL;
RPOR_RP2R = OC1; // map OC1 to pin 6
OC1R = RESET; //very important
OC1RS = OCxRS_VAL;

TMR3 = RESET;
PR3 = PR3_VAL;
OC2CONbits.OCTSEL = ON; // using timer 3
OC2CONbits.OCM = OCxCON_VAL;
RPOR_RP8R = OC2; // map OC2 to pin 17
OC2R = RESET; //very important
OC2RS = OCxRS_VAL;
T3CONbits.TON = ON;

}

// this function initializes the pulse width modulation for backwards movement
void initBackPWM(){

RPOR_RP2R = RESET; // map OC1 to pin 6
RPOR_RP8R = RESET; // map OC2 to pin 17

LAT_B2 = RESET; // Pin 6 to low to go backwards
LAT_B8 = RESET; // Pin 17 to output to go backwards

OC1CONbits.OCM = RESET;
OC2CONbits.OCM = RESET;
T3CONbits.TCKPS = T3_PR;
TMR3 = RESET;
PR3 = PR3_VAL;
OC1CONbits.OCTSEL = ON; // using timer 3
OC1CONbits.OCM = OCxCON_VAL;
RPOR_RP3R = OC1; // map OC1 to pin 7
OC1R = RESET; //very important
OC1RS = OCxRS_VAL;


OC2CONbits.OCTSEL = ON; // using timer 3
OC2CONbits.OCM = OCxCON_VAL;
RPOR_RP9R = OC2; // map OC2 to pin 18
OC2R = RESET; //very important
OC2RS = OCxRS_VAL;
T3CONbits.TON = ON;

}