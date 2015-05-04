/* 
 * File:   i2c.h
 * Author: Steven
 *
 * Created on May 3, 2015, 4:43 PM
 */

#ifndef I2C_H
#define	I2C_H

void initI2C();
void sendI2C(char data, char address);
char receiveI2C(char address);

#endif	/* I2C_H */

