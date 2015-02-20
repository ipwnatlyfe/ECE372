/* 
 * File:   timer.h
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */
#include <stdio.h>
#ifndef INITTIMER_H
#define	INITTIMER_H

void delayUs(unsigned int delay);
void Timer1Delay(unsigned int delay);
char* getTimeString(unsigned int c);
void startTime();

#endif	/* INITTIMER_H */

