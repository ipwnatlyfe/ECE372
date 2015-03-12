/* 
 * File:   keypad.h
 * Author: user
 *
 * Created on February 5, 2015, 11:42 AM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

#define COL2 PORTBbits.RB10 // pin 21
#define COL1 PORTBbits.RB7 // pin 18
#define COL3 PORTBbits.RB11 // pin 22

#define ROW1 LATAbits.LATA0 //pin 23
#define ROW2 LATAbits.LATA1 //pin 26
#define ROW3 LATBbits.LATB2 //pin 25
#define ROW4 LATBbits.LATB3 //pin 24

#define KEY_PRESSED 0
#define KEY_RELEASE 1

void initKeypad(void);
char scanKeypad(void);

#endif	/* KEYPAD_H */

