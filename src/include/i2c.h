#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void init_i2c(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t dane);
uint8_t i2c_read(uint8_t ACK);

#endif
