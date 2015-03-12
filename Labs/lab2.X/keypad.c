#include "p24FJ64GA002.h"
#include "keypad.h"
#include "timer.h"

#define ON 1
#define OFF 0

// number of rows and columns defined on keypad
#define ROWS 4
#define COLS 3

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){


// RB15 to RB12 (LED4 to LED7) set as outputs to both LCD and Keypad
TRISBbits.TRISB15 = 0;
TRISBbits.TRISB14 = 0;
TRISBbits.TRISB13 = 0;
TRISBbits.TRISB12 = 0;

/*
// Set RB3 (pin7 IO6), RB10 (pin21 IO9) and RB11 (pin22 IO10) as inputs from the Keypad presses
TRISBbits.TRISB3 = ON; //pin7 RB3 IO6
TRISBbits.TRISB10 = ON; //pin21 RB10 IO9
TRISBbits.TRISB11 = ON; //pin22 RB11 IO10
 */

// Set RB9 (pin18 RX/PGC), RB10 (pin21 IO9) and RB11 (pin22 IO10) as inputs from the Keypad presses
TRISBbits.TRISB9 = ON; //pin18 RB9 RX/PGC
TRISBbits.TRISB10 = ON; //pin21 RB10 IO9
TRISBbits.TRISB11 = ON; //pin22 RB11 IO10

// initialize rows as ODC outputs
ODCAbits.ODA0 = ON; //pin 23
ODCAbits.ODA1 = ON; //pin 26
ODCBbits.ODB2 = ON; //pin 25
ODCBbits.ODB3 = ON; //pin 24

/*
//When do i know to set pins as digital inputs?
// Set RB3 to digital input pin
//which one is RB3? the other is RB2
AD1PCFGbits.PCFG4 = 1;
AD1PCFGbits.PCFG5 = 1;
*/

// initialize columns as inputs with pull up resistors
CNPU2bits.CN21PUE = COL1; // pin 18 as pull up resistor
CNPU2bits.CN16PUE = COL2; // pin 21 "
CNPU1bits.CN15PUE = COL3; // pin 22 "

// CN Interrupt flag set to 0 to detect first interrupt
IFS1bits.CNIF = 0;

//enable flag interrupt
IEC1bits.CNIE = 1;

// CN Interrupt keypad button outputs enabled
CNEN1bits.CN15IE = 1; // pin 22 CN enabled
CNEN2bits.CN16IE = 1; // pin 21 CN enabled
CNEN2bits.CN21IE = 1; // pin 18 CN enabled

// Ground rows of keypad
LATB &= 0x0FFF;
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 *
 *
 */
char scanKeypad(void){

    char key = -1;
/*
    //set a latch == 1... ie) ROW1 == 1
    //if (COL1 == 1) then we have found the number
    //if not, then set ROW1 == 0, ROW2 == 1 try again

    ROW1 = 1;
    ROW2 = 0;
    ROW3 = 0;
    ROW4 = 0;

    if (COL1 == 1)
    {
        key = '1';
    }

    else if (COL2 == 1)
    {
        key = '2';
    }

    else if (COL3 == 1)
    {
        key = '3';
    }

    ROW1 = 0;
    ROW2 =1;
    if (COL1 == 1)
    {
        key = '4';
    }

    else if (COL2 == 1)
    {
        key = '5';
    }

    else if (COL3 == 1)
    {
        key = '6';
    }

    ROW2 = 0;
    ROW3 =1;
    if (COL1 == 1)
    {
        key = '7';
    }

    else if (COL2 == 1)
    {
        key = '8';
    }

    else if (COL3 == 1)
    {
        key = '9';
    }

    ROW3 = 0;
    ROW4 =1;
    if (COL1 == 1)
    {
        key = '*';
    }

    else if (COL2 == 1)
    {
        key = '0';
    }

    else if (COL3 == 1)
    {
        key = '#';
    }

    
    return key;

 */


    // Variables to hold
    char row = -1, column = -1;

    // Temp variable iterating in rows
    char i;

    // configuration of keypad
    char keyValue[ROWS][COLS] =
    {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
    };

    // Temporary hold value of LATB to restore later
    unsigned int temp = LATB;

    // Now we will detect if exactly one button is pressed.
    // Function legend:
    //
    //      -1          : if no keys are pressed.
    //                  : if multiple keys pressed simulatneously
    //      '0' to '9'  : '0' to '9' if a numeric key is pressed.
    //      '#'         : if the # is pressed.
    //      '*'         : if the * key is pressed
    //
    // Notes:
    //        1. Only valid inputs should be allowed by the user such that all invalid inputs
    //           are ignored until a valid input is detected.
    //        2. The user must release all keys of the keypad before the following key press
    //           is processed. This is to prevent invalid keypress from being processed if the
    //           users presses multiple keys simultaneously.

    // Ground all of the rows of the keypad
    LATB &= 0x0FFF;

    // Check if any keys are pressed at all
    if((PORTB & 0x040C) == 0x040C)
    {
        // No key pressed
        key = -1;
    }

    else
    {
        // Find out which key has been pressed
	// Iterate through the rows, grounding each one
	for(i = 0; i < ROWS; i++)
        {
            // Unground all of the rows
            LATB |= 0xF000;
            
            // Ground only the i'th row by toggling
            LATB ^= ((0x1000)<<i);

            // Delay for 1 microsecond to allow bits to change in LATB
            delayUs(1);

            // Check if we've found a match
            if((PORTB & 0x040C) != 0x040C)
            {
                // Set row variable to the row number where we found a press
		row = i;

		// Figure out which column the button is in
		switch((PORTB & 0x040C))
                {
                    case 0x0408: // Column 1 pressed
                        column = 0;
                        break;
                    case 0x0404: // Column 2 pressed
			column = 1;
                        break;
                    case 0x000C: // Column 3 pressed
			column = 2;
			break;
                    default:	// Fatal error has occurred
			break;
		}
            }
        }

        if(row == -1)
        {
            key = 0x21; // Display a '!' when we encounter an error with row
        }

        else if(column == -1)
        {
            key = 0x26; // Display a '&' when we encounter error with column
        }

        else
        {
            // No error: set key to appropriate character
            key = keyValue[row][column];
        }

        // Ground all of rows to make sure user lets go of buttons
	LATB &= 0x0FFF;

	// Wait for user to let go of buttons before continuing execution
	while((PORTB & 0x040C) != 0x040C);
    }

    // Set LATB back to previous state
    LATB = temp;

    return key;
}
