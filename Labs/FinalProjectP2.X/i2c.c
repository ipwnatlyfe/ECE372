#include "p24FJ64GA002.h"

#define WRITE 0
#define READ 1

void initI2C(){
//    Map the pins properly
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0; // probbably not necessary

    I2C1CONbits.I2CEN = 1;
    IFS1bits.MI2C1IF = 0;
    I2C1BRG = 10000;
}

void sendI2C(char data, char address){
    //Start bit
    I2C1CONbits.SEN = 1;
    //wait
    while(IFS1bits.MI2C1IF == 0);
    IFS1bits.MI2C1IF = 0;
    //Address
    I2C2TRN = address << 1 | WRITE;
    //wait
    while(I2C1STATbits.TRSTAT == 1);
    //Check for Ack
    if(I2C1STATbits.ACKSTAT == 1);
    //Data
    I2C1TRN = data;
    //wait
    while(I2C1STATbits.TRSTAT == 1);
    //Check for Ack
    if(I2C1STATbits.ACKSTAT == 1);
    //Stop
    I2C1CONbits.PEN = 1;
    //wait
    while(IFS1bits.MI2C1IF == 0);
    IFS1bits.MI2C1IF = 0;
}

char receiveI2C(char address){
    char c;
    //Start bit
    I2C1CONbits.SEN = 1;
    //wait
    while(IFS1bits.MI2C1IF == 0);
    IFS1bits.MI2C1IF = 0;
    //Address
    I2C1TRN = address << 1 | READ;
    //wait
    while(I2C1STATbits.TRSTAT == 1);
    //Check for Ack
    if(I2C1STATbits.ACKSTAT == 1);
    //wait
    while(I2C1STATbits.RBF == 0);
    //Check for Ack
    c = I2C1RCV;
    //Acknowledgement
    I2C1CONbits.ACKDT = 1; //ACK or NACK
    I2C1CONbits.ACKEN = 1; //enable acknowledge
    while(IFS1bits.MI2C1IF == 0);
    IFS1bits.MI2C1IF = 0;
    //Stop
    I2C1CONbits.PEN = 1;
    //wait
    while(IFS1bits.MI2C1IF == 0);
    IFS1bits.MI2C1IF = 0;
    return c;
}
