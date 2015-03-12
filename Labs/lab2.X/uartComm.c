/*
* File:   uartComm.h
* Author: garrettvanhoy
*
* Created on December 27, 2014, 12:13 PM
*/

// ******************************************************************************************* //
// Defines to simply UART's baud rate generator (BRG) regiser
// given the osicllator freqeuncy and PLLMODE.

#define XTFREQ          7372800         	  // On-board Crystal frequency
#define PLLMODE         4               	  // On-chip PLL setting (Fosc)
#define FCY             (XTFREQ*PLLMODE)/2        // Instruction Cycle Frequency (Fosc/2)

#define BAUDRATE         115200
#define BRGVAL          ((FCY/BAUDRATE)/16)-1
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "uartComm.h"
#include <stdio.h>

void initUART(){

    //Peripheral Pin Mapping
    RPINR18bits.U1RXR = 9;
    RPOR4bits.RP8R = 3;

    //Configuring the UART
    U1BRG  = BRGVAL;
    U1MODE = 0x8000;
    U1STA  = 0x0440;

    //Putting the UART interrupt flag down.
    IFS0bits.U1RXIF = 0;
}

void waitForChar(){
    int receivedChar;
    // Use the UART RX interrupt flag to wait until we recieve a character.
    while(IFS0bits.U1RXIF == 1){
        // Clear the UART RX interrupt flag to we can detect the reception
        // of another character.
        IFS0bits.U1RXIF = 0;
        // U1RXREG stores the last character received by the UART. Read this
        // value into a local variable before processing.
        receivedChar = U1RXREG;
        // Echo the entered character so the user knows what they typed.
        printf("%c\n\r", receivedChar);
    }
}
