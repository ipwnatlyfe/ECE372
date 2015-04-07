/*
 * File:   lcd.c
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"

#define LCD_DATA   LATB
#define LCD_RS  LATBbits.LATB7
#define LCD_E   LATBbits.LATB6

#define TRIS_D7  TRISBbits.TRISB15
#define TRIS_D6  TRISBbits.TRISB14
#define TRIS_D5  TRISBbits.TRISB13
#define TRIS_D4  TRISBbits.TRISB12
#define TRIS_RS  TRISBbits.TRISB7
#define TRIS_E   TRISBbits.TRISB6

#define LOWER 1
#define UPPER 0

#define LCD_WRITE_DATA 1
#define LCD_WRITE_CONTROL 0

/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATB. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower){
    //TODO:
    if(lower)
    {
        LCD_DATA = (0x0FFF & LCD_DATA) | ((word & 0x0F)<<12);
    }
    else
    {
        LCD_DATA = (0x0FFF & LCD_DATA) | ((word & 0xF0)<<8);
    }
    LCD_RS = commandType;
    delayUs(1);
    LCD_E = 1;
    delayUs(1);
    LCD_E = 0;
    delayUs(1);
    delayUs(delayAfter);
}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter){
    //TODO:
    writeFourBits(word, commandType, delayAfter, UPPER);
    writeFourBits(word, commandType, delayAfter, LOWER);
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {
    //TODO:
    writeLCD(c, LCD_WRITE_DATA, 46);
}
/*Initialize the LCD
 */
void initLCD(void) {
    // Setup D, RS, and E to be outputs (0).
    TRIS_RS = 0;
    TRIS_E = 0;
    TRIS_D7 = 0;
    TRIS_D6 = 0;
    TRIS_D5 = 0;
    TRIS_D4 = 0;


    // Initilization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initilition commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.
    delayUs(15000);
    // Enable 4-bit interface
    writeFourBits(0x03, LCD_WRITE_CONTROL, 4100, LOWER);
    writeFourBits(0x03, LCD_WRITE_CONTROL, 100, LOWER);
    writeLCD(0x32, LCD_WRITE_CONTROL, 100);








    // Function Set (specifies data width, lines, and font.
    writeLCD(0x28, LCD_WRITE_CONTROL, 40);

    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    // TODO: Display On/Off Control
        // Turn Display (D) Off
    writeLCD(0x08, LCD_WRITE_CONTROL, 40);

    // TODO: Clear Display (The delay is not specified in the data sheet at this point. You really need to have the clear display delay here.
    writeLCD(0x01, LCD_WRITE_CONTROL, 1640);

    // TODO: Entry Mode Set
        // Set Increment Display, No Shift (i.e. cursor move)
    writeLCD(0x06, LCD_WRITE_CONTROL, 40);
    // TODO: Display On/Off Control
        // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
    writeLCD(0x0C, LCD_WRITE_CONTROL, 40);

}

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) {
    //TODO:
    int i =0;
    while(s[i] != '\0')
    {
        printCharLCD(s[i]);
        i++;
    }
}

/*
 * Clear the display.
 */
void clearLCD(){
    writeLCD(0x01, LCD_WRITE_CONTROL, 1640);
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char y, unsigned char x){
    char Address = 0x00;
    char send= 0x01;
    send = send << 7;
    if (y == 1)
    {
        Address = Address + 0x40;
    }

    else if (y == 0)
    {
        Address = Address;
    }

    Address = Address + (x);

    send = (Address | send);
    writeLCD(send, LCD_WRITE_CONTROL, 40);
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayUs(1000);
}
