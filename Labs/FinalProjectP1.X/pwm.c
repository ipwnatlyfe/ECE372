#include "p24FJ64GA002.h"
#include "pwm.h"

#define TRIS_B3 TRISBbits.TRISB3
#define TRIS_B0 TRISBbits.TRISB0
#define TRIS_B11 TRISBbits.TRISB11
#define TRIS_B10 TRISBbits.TRISB10

#define LAT_B11 LATBbits.LATB11
#define LAT_B10 LATBbits.LATB10
#define LAT_B3 LATBbits.LATB3
#define LAT_B0 LATBbits.LATB0

#define RPOR_RP10R RPOR5bits.RP10R
#define RPOR_RP3R RPOR1bits.RP3R
#define RPOR_RP11R RPOR5bits.RP11R
#define RPOR_RP0R RPOR0bits.RP0R

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
RPOR_RP0R = RESET; // map OC2 to pin 18

LAT_B3 = RESET; // Set pin 7 to low to create a voltage difference
LAT_B0 = RESET; // Set pin 18 to low to create a voltage difference

OC1CONbits.OCM = RESET;
OC2CONbits.OCM = RESET;
T3CONbits.TCKPS = T3_PR;
TMR3 = RESET;
PR3 = PR3_VAL;
OC1CONbits.OCTSEL = ON; // using timer 3
OC1CONbits.OCM = OCxCON_VAL;
RPOR_RP10R = OC1; // map OC1 to pin 21
OC1R = RESET; //very important
OC1RS = OCxRS_VAL;

TMR3 = RESET;
PR3 = PR3_VAL;
OC2CONbits.OCTSEL = ON; // using timer 3
OC2CONbits.OCM = OCxCON_VAL;
RPOR_RP11R = OC2; // map OC2 to pin 17
OC2R = RESET; //very important
OC2RS = OCxRS_VAL;
T3CONbits.TON = ON;

}

void initPWMSpin(){

RPOR_RP3R = RESET;
RPOR_RP11R = RESET;


LAT_B3 = RESET; // Set pin 7 to low to create a voltage difference
LAT_B11 = RESET;


OC1CONbits.OCM = RESET;
OC2CONbits.OCM = RESET;
T3CONbits.TCKPS = T3_PR;
TMR3 = RESET;
PR3 = PR3_VAL;
OC1CONbits.OCTSEL = ON; // using timer 3
OC1CONbits.OCM = OCxCON_VAL;
RPOR_RP10R = OC1; // map OC1 to pin 21
OC1R = RESET; //very important
OC1RS = OCxRS_VAL;

TMR3 = RESET;
PR3 = PR3_VAL;
OC2CONbits.OCTSEL = ON; // using timer 3
OC2CONbits.OCM = OCxCON_VAL;
RPOR_RP0R = OC2; // map OC2 to pin 17
OC2R = RESET; //very important
OC2RS = OCxRS_VAL;
T3CONbits.TON = ON;

}


