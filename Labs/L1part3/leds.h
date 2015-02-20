/* 
* File:   leds.h
* Author: 
*
* Created on December 27, 2014, 1:31 PM
*/

#ifndef LEDS_H
#define	LEDS_H

#define LED_RUN LATAbits.LATA0
#define LED_STOP LATAbits.LATA1

#define ON 0
#define OFF 1

void initLEDs();
void initSW2();

#endif	/* LEDS_H */

