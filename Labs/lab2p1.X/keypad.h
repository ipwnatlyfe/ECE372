/* 
 * File:   keypad.h
 * Author: user
 *
 * Created on February 5, 2015, 11:42 AM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

//Rows
#define ROW1 LATAbits.LATA0
#define ROW2 LATAbits.LATA1
#define ROW3 LATBbits.LATB2
#define ROW4 LATBbits.LATB9

//Columns
#define COL1 PORTBbits.RB8
#define COL2 PORTBbits.RB10
#define COL3 PORTBbits.RB11

#define ON 0
#define OFF 1

#define OUTPUT 0
#define INPUT 1

#define PRESSED 1
#define NOTPRESSED 0

#define HI = 1
#define LOW = 0

void initKeypad(void);
char scanKeypad(void);

#endif	/* KEYPAD_H */

