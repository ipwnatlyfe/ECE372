// ******************************************************************************************* //
// File:         lab2p1.c
// Date:         
// Authors:      
//
// Description: 
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include <stdio.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

typedef enum stateTypeENUM{
    WAIT, START
} stateType;

volatile stateType currstate;
initLCD();
initKeypad();


int main(void)
{
    currstate = WAIT;
    while(1){
        testLCD();
        switch (currstate)
        {

        }
    }
    
    return 0;
}

void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;
    delayUs(5000);
    int ButtonPushed = 0;

    //Check to see that the button is still pushed meaning at least one of the inputs
    //is low voltage
    if((LATBbits.LATB7 == 0) || (LATBbits.LATB10 == 0) || (LATBbits.LATB11 == 0))
    {
        ButtonPushed = 1;
    }
    else
    {
        ButtonPushed = 0;
    }

    if(currstate == WAIT && ButtonPushed == 1)
    {
        currstate = START;
    }
    else if(currstate == WAIT && ButtonPushed == 0)
    {
        ButtonPushed = 0;
    }
    else if(currstate == START && ButtonPushed == 1)
    {
        int checkChar = scanKeypad();
        if(checkChar != -1)
        {
            printCharLCD(checkChar);
        }
        currstate = WAIT;
    }
    
}