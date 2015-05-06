/* 
 * File:   i2c.h
 * Author: Steven
 *
 * Created on May 4, 2015, 8:52 PM


#ifndef I2C_H
#define	I2C_H



void i2c_init(int BRG);
void i2c_start(void);
void i2c_restart(void);
void reset_i2c_bus(void);
char send_i2c_byte(int data);
char i2c_read(void);
char i2c_read_ack(void);
void I2Cwrite(char addr, char subaddr, char value);
char I2Cread(char addr, char subaddr);
unsigned char I2Cpoll(char addr);
*/
/*
 * File:   i2c.h
 * Author: user
 *
 * Created on March 10, 2015, 9:08 AM
 */

#ifndef I2C_H
#define	I2C_H

void initI2C();
void sendI2C(char data, char address);
char receiveI2C(char address);
char get_data(char i2c_address, char data_address);
void writeI2C( char address, wchar_t subaddr, char data );
#endif	/* I2C_H */
