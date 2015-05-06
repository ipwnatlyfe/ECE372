/* 
 * File:   RangeFinder.c
 * Author: Steven
 *
 * Created on May 4, 2015, 7:32 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24fj64ga002.h"
#include "i2c.h"
#include "lcd.h"
#include "timer.h"

#define addr 0x29

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

volatile int counter =0;

int VL6180X_Start_Range() {
 writeI2C(addr, 0x018, 0x01);
 return 0;
}

int VL6180X_Poll_Range() {
 char status;
 char range_status;

 // check the status
 status = get_data(addr,0x04f);
 range_status = status & 0x07;

 // wait for new measurement ready status
 while (range_status != 0x04) {
 status = get_data(addr,0x04f);
 range_status = status & 0x07;
 //delayUs(1000); // (can be removed)
 }
 return 0;
}

int VL6180X_Read_Range() {
    int range;
 range=get_data(addr,0x062);
 return range;
}

int VL6180X_Clear_Interrupts() {
 writeI2C(addr,0x015,0x07);
 return 0;
}

int main(void)
{
    initLCD();
    initI2C();
    int range;
    static char str[15];


    writeI2C(addr, 0x0207, 0x01);
    writeI2C(addr,0x0208, 0x01);
    writeI2C(addr,0x0096, 0x00);
    writeI2C(addr,0x0097, 0xfd);
    writeI2C(addr,0x00e3, 0x00);
    writeI2C(addr,0x00e4, 0x04);
    writeI2C(addr,0x00e5, 0x02);
    writeI2C(addr,0x00e6, 0x01);
    writeI2C(addr,0x00e7, 0x03);
    writeI2C(addr,0x00f5, 0x02);
    writeI2C(addr,0x00d9, 0x05);
    writeI2C(addr,0x00db, 0xce);
    writeI2C(addr,0x00dc, 0x03);
    writeI2C(addr,0x00dd, 0xf8);
    writeI2C(addr,0x009f, 0x00);
    writeI2C(addr,0x00a3, 0x3c);
    writeI2C(addr,0x00b7, 0x00);
    writeI2C(addr,0x00bb, 0x3c);
    writeI2C(addr,0x00b2, 0x09);
    writeI2C(addr,0x00ca, 0x09);
    writeI2C(addr,0x0198, 0x01);
    writeI2C(addr,0x01b0, 0x17);
    writeI2C(addr,0x01ad, 0x00);
    writeI2C(addr,0x00ff, 0x05);
    writeI2C(addr,0x0100, 0x05);
    writeI2C(addr,0x0199, 0x05);
    writeI2C(addr,0x01a6, 0x1b);
    writeI2C(addr,0x01ac, 0x3e);
    writeI2C(addr,0x01a7, 0x1f);
    writeI2C(addr,0x0030, 0x00);

    writeI2C(addr,0x0011, 0x10);
    writeI2C(addr,0x010a, 0x30);
    writeI2C(addr,0x003f, 0x46);
    writeI2C(addr,0x0031, 0xFF);
    writeI2C(addr,0x0040, 0x63);
    writeI2C(addr,0x002e, 0x24);

    while (1){

       range=get_data(0x29, 0x0000);

        
         // start single range measurement
         VL6180X_Start_Range();

         // poll the VL6180X till new sample ready
         VL6180X_Poll_Range();

         // read range result
         range = VL6180X_Read_Range();

         // clear the interrupt on VL6180X
         VL6180X_Clear_Interrupts();

         /*
         moveCursorLCD(0,0);
         sprintf(str, "%d", range);
         printStringLCD(str);
        */
         delayUs(10);
         
 }

    return 0;
}


