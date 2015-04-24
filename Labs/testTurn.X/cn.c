
#include "p24fj64ga002.h"
#include "cn.h"

#define TRIS_B5 TRISBbits.TRISB5
#define ON 1

// this function enables the change notification interrupt for switch 1 on the pic
void initCNForSW1(){
    //Use the switch on the board connected to RB5
    TRIS_B5 = ON;
   // IFS1bits.CNIF = 1;
    IEC1bits.CNIE = ON; //enable interrupt
    CNEN2bits.CN27IE = ON; // enable cn on rb5/sw1
}