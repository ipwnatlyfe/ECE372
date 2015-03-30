#include "p24FJ64GA002.h"
#include "pwm.h"
void initPWM(){

T2CONbits.TCKPS = 3;
TMR2 = 0;
PR2 = 56;
OC1CONbits.OCTSEL = 1; // using timer 3
OC1CONbits.OCM = 6;
RPOR5bits.RP11R = 18; // map OC1 to pin 22
OC1R = 0; //very important
OC1RS = 14;
T2CONbits.TON = 1;

}
