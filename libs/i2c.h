#ifndef __I2C_H
#define __I2C_H

#include <msp430.h>


void i2cConfig();
void i2cWriteByte(uint8_t addr, uint8_t byte);
uint8_t i2cWrite(uint8_t addr,uint8_t * data, uint8_t nBytes);

#endif  // __I2C_H
