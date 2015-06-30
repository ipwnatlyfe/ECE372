#include "p24FJ64GA002.h"

#define WRITE 0
#define READ 1

void initI2C(){
//    Map the pins properly
    int ClearBuffer;

    I2C1CONbits.I2CEN = 0;  //Disable I2C
    I2C1CONbits.DISSLW = 0; //Disable Slew Rate
    IFS1bits.MI2C1IF = 0;   //Clear Flag
    I2C1CONbits.I2CEN = 1;  //Enable I2C
    ClearBuffer = I2C1RCV;  //Read Buffer to Clear it
    I2C1BRG = 10000;
}

void startI2C(){
    int x = 0;
    I2C1CONbits.ACKDT = 0;  //Clears Ack bit
    delayUs(10);            //Delays for reset
    I2C1CONbits.SEN = 1;    //Start condition

    while (I2C1CONbits.SEN){//Apparently we have to wait until the start bit clears
        x++;
        delayUs(1);
        if(x>30)
        {
            break; //makes sure that no endless loops are possible
        }
    }
}

void restartI2C(){
    int x = 0;
    I2C1CONbits.RSEN = 1;

    while (I2C1CONbits.RSEN){//Apparently we have to wait until the restart bit clears
        x++;
        delayUs(1);
        if(x>30)
        {
            break; //makes sure that no endless loops are possible
        }
    }
}



void sendI2C(char data, char address){
    //Start bit
    I2C1CONbits.SEN = 1;
    //wait
    while(IFS1bits.MI2C1IF == 0);
    IFS1bits.MI2C1IF = 0;
    //Address
    I2C1TRN = address << 1 | WRITE;
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
//Had to modify this to work with the supplied version of I2C communication
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
