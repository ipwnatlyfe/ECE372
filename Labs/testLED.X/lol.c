#include "p24fj64ga002.h"
#include "leds.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

int main(void)
{
    initLEDs();
    while(1)
    {
        LATBbits.LATB13 = 1;
        LATBbits.LATB14 = 1;
        LATBbits.LATB15 = 1;
    }
 return 0;
}