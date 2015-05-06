/* 
 * File:   i2c.c
 * Author: Steven
 *
 * Created on May 4, 2015, 8:52 PM
 */
/*
#include <stdio.h>
#include <stdlib.h>
#include "p24fj64ga002.h"
#include <math.h>
//#define addr 0x29

unsigned char temp;
*/
/*
 * 
 */
//function initiates I2C1 module to baud rate BRG


//function iniates a start condition on bus


/*
void i2c_start(void)
{
   int x = 0;
   I2C2CONbits.ACKDT = 0;	//Reset any previous Ack
   delayUs(10);
   I2C2CONbits.SEN = 1;	//Initiate Start condition


   //the hardware will automatically clear Start Bit
   //wait for automatic clear before proceding
   while (I2C2CONbits.SEN)
   {
      delayUs(1);
      x++;
      if (x > 20)
      break;
   }
   delayUs(2);
}

void i2c_restart(void)
{
   int x = 0;

   I2C2CONbits.RSEN = 1;	//Initiate restart condition


   //the hardware will automatically clear restart bit
   //wait for automatic clear before proceding
   while (I2C2CONbits.RSEN)
   {
      delayUs(1);
      x++;
      if (x > 20)	break;
   }

   delayUs(2);
}

//Resets the I2C bus to Idle
void reset_i2c_bus(void)
{
   int x = 0;

   //initiate stop bit
   I2C2CONbits.PEN = 1;

   //wait for hardware clear of stop bit
   while (I2C2CONbits.PEN)
   {
      delayUs(1);
      x ++;
      if (x > 20) break;
   }
   I2C2CONbits.RCEN = 0;
   IFS3bits.MI2C2IF = 0; // Clear Interrupt
   I2C2STATbits.IWCOL = 0;
   I2C2STATbits.BCL = 0;
   delayUs(10);
}

//basic I2C byte send
char send_i2c_byte(int data)
{
   int i;

   while (I2C2STATbits.TBF) { }
   IFS3bits.MI2C2IF = 0; // Clear Interrupt
   I2C2TRN = data; // load the outgoing data byte

   // wait for transmission
   for (i=0; i<500; i++)
   {
      if (!I2C1STATbits.TRSTAT) break;
      delayUs(1);

      }
      if (i == 500) {
      return(1);
   }

   // Check for NO_ACK from slave, abort if not found
   if (I2C2STATbits.ACKSTAT == 1)
   {
      reset_i2c_bus();
      return(1);
   }

       delayUs(2);
   return(0);
}

//function reads data, returns the read data, no ack
char i2c_read(void)
{
   int i = 0;
   char data = 0;

   //set I2C module to receive
   I2C2CONbits.RCEN = 1;

   //if no response, break
   while (!I2C2STATbits.RBF)
   {
      i ++;
      if (i > 2000) break;
   }

   //get data from I2CRCV register
   data = I2C2RCV;

   //return data
   return data;
}

//function reads data, returns the read data, with ack
char i2c_read_ack(void)	//does not reset bus!!!
{
   int i = 0;
   char data = 0;

   //set I2C module to receive
   I2C2CONbits.RCEN = 1;

   //if no response, break
   while (!I2C2STATbits.RBF)
   {
      i++;
      if (i > 2000) break;
   }

   //get data from I2CRCV register
   data = I2C2RCV;

   //set ACK to high
   I2C2CONbits.ACKEN = 1;

   //wait before exiting
   delayUs(10);

   //return data
   return data;
}

void I2Cwrite(char addr, wchar_t subaddr, char value)
{
     char data_write[3];
     data_write[0] = (subaddr >> 8) & 0xFF;; // MSB of register address
     data_write[1] = subaddr & 0xFF; // LSB of register address
     data_write[2] = subaddr & 0xFF;

   i2c_start();
   send_i2c_byte(addr);
   send_i2c_byte(data_write);
   send_i2c_byte(value);
   reset_i2c_bus();
}

char I2Cread(char addr, char subaddr)
{
   char temp;

   i2c_start();
   send_i2c_byte(addr);
   send_i2c_byte(subaddr);
   delayUs(10);

   i2c_restart();
   send_i2c_byte(addr | 0x01);
   temp = i2c_read();

   reset_i2c_bus();
   return temp;
}

unsigned char I2Cpoll(char addr)
{
   unsigned char temp = 0;

   i2c_start();
   temp = send_i2c_byte(addr);
   reset_i2c_bus();

   return temp;
}

void i2c_init(int BRG)
{
   int temp;

    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0; // probbably not necessary
    AD1PCFGbits.PCFG4 = 1;
    AD1PCFGbits.PCFG5 = 1;

   // I2CBRG = 194 for 10Mhz OSCI with PPL with 100kHz I2C clock
   I2C2BRG = BRG;
   I2C2CONbits.I2CEN = 0;	// Disable I2C Mode
   I2C2CONbits.DISSLW = 1;	// Disable slew rate control
   IFS3bits.MI2C2IF = 0;	// Clear Interrupt
   I2C2CONbits.I2CEN = 1;	// Enable I2C Mode
   temp = I2C2RCV;              // read buffer to clear buffer full


    

}

*/
/*
 * File:   i2c.c
 * Author: gvanhoy
 *
 * Created on March 10, 2015, 9:08 AM
 */
#define WRITE 0
#define READ 1
#include <stdio.h>
#include <stdlib.h>
#include "p24fj64ga002.h"
#include <math.h>
#include "lcd.h"
#include "timer.h"
void initI2C(){
//    Map the pins properly
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0; // probably not necessary
    AD1PCFGbits.PCFG4 = 1;
    AD1PCFGbits.PCFG5 = 1;

    I2C2CONbits.I2CEN = 1;
    IFS3bits.MI2C2IF = 0;
    I2C2BRG = 10000;
    AD1PCFG = 0xFFFF;
}

void sendI2C(char data, char address){
    //Start bit
    I2C2CONbits.SEN = 1;
    //wait
    while(I2C2CONbits.SEN);
    //Address
    I2C2TRN = (address << 1) | WRITE;
    //wait
    while(I2C2STATbits.TRSTAT == 1);
    //Check for Ack
    if(I2C2STATbits.ACKSTAT == 1);
    //Data
    I2C2TRN = data;
    //wait
    while(I2C2STATbits.TRSTAT == 1);
    //Check for Ack
    if(I2C2STATbits.ACKSTAT == 1);
    //Stop
    I2C2CONbits.PEN = 1;
    //wait
    while(I2C2CONbits.PEN);
}

void writeI2C( char address, wchar_t subaddr, char data ){
    char data_write[3];
     data_write[0] = (subaddr >> 8) & 0xFF;; // MSB of register address
     data_write[1] = subaddr & 0xFF; // LSB of register address
     data_write[2] = subaddr & 0xFF;
    //Start bit
    I2C2CONbits.SEN = 1;
    //wait
    while(I2C2CONbits.SEN);
    //Address
    I2C2TRN = (address << 1) | WRITE;
    //wait
    while(I2C2STATbits.TBF || I2C2STATbits.TRSTAT);
    //Check for Ack
    if(I2C2STATbits.ACKSTAT == 1);
    //MSB of sub address
    I2C2TRN = data_write[0];
    //wait
    while(I2C2STATbits.TBF || I2C2STATbits.TRSTAT);
    //Check for Ack
    if(I2C2STATbits.ACKSTAT == 1);
    //LSB of sub address
    I2C2TRN = data_write[1];
    //wait
    while(I2C2STATbits.TBF || I2C2STATbits.TRSTAT);
    //Check for Ack
    if(I2C2STATbits.ACKSTAT == 1);
    //Data
    I2C2TRN = data;
    //wait
    while(I2C2STATbits.TBF || I2C2STATbits.TRSTAT);
    //Check for Ack
    if(I2C2STATbits.ACKSTAT == 1);
    //Stop
    I2C2CONbits.PEN = 1;
    //wait
    while(I2C2CONbits.PEN);
}


char receiveI2C(char i2c_address){
    char c;
    //Start bit
    I2C2CONbits.SEN = 1;
    //wait
    while(I2C2CONbits.SEN);
    //Address
    I2C2TRN = (i2c_address << 1) | READ;
    //wait
    while(I2C2STATbits.TRSTAT == 1);
    //Check for Ack
    if(I2C2STATbits.ACKSTAT == 1);
    //wait
    while(I2C2STATbits.RBF == 0);
    //Check for Ack
    c = I2C2RCV;
    //Acknowledgement
    I2C2CONbits.ACKDT = 1; //ACK or NACK
    I2C2CONbits.ACKEN = 1; //enable acknowledge
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    //Stop
    I2C2CONbits.PEN = 1;
    //wait
    while(I2C2CONbits.PEN);
    return c;
}

char get_data(char i2c_address, int data_address){
    char c;
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN);
    I2C2TRN = i2c_address << 1 | WRITE;
    while(I2C2STATbits.TBF || I2C2STATbits.TRSTAT);
    I2C2TRN = data_address >> 8;
    while(I2C2STATbits.TBF || I2C2STATbits.TRSTAT);
    I2C2TRN = (data_address << 8) >> 8;
    while(I2C2STATbits.TBF || I2C2STATbits.TRSTAT);
    I2C2CONbits.PEN = 1;
    //wait
    while(I2C2CONbits.PEN);
    I2C2CONbits.SEN = 1;
    //wait
    while(I2C2CONbits.SEN);
    I2C2TRN = (i2c_address << 1) | READ;
    while(I2C2STATbits.TBF || I2C2STATbits.TRSTAT);
//    while(I2C2STATbits.ACKSTAT);
    I2C2CONbits.RCEN = 1;
    while(I2C2STATbits.RBF == 0);
    c = I2C2RCV;
    I2C2CONbits.ACKDT = 1; //ACK or NACK
    I2C2CONbits.ACKEN = 1; //enable acknowledge
    while(I2C2CONbits.PEN);
    printStringLCD("hi");
    return c;
}



