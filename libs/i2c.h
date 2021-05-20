#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>

uint8_t i2cWrite(uint8_t addr, uint8_t * data, uint8_t nBytes);
void i2cConfig();
void i2cWriteByte(uint8_t addr, uint8_t byte);

#endif
