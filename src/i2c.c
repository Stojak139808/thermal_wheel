#include "include/mcu.h"
#include <stdint.h>
#include <avr/io.h>

typedef enum{
    I2C_DIV1,
    I2C_DIV4,
    I2C_DIV16,
    I2C_DIV64
}i2c_div_t;

static void set_i2c_prescaler(i2c_div_t divider);
static void set_i2c_bitrate(uint8_t bitrate);

static inline void i2c_wait_for_twint(void);

void init_i2c(){
    set_i2c_prescaler(I2C_DIV16);
    /*
     * max allowed frequency for BMP280 is 3MHz, set it to 333kHz
     * because max allowed speed for atmega328p is 400kHz
     */
    set_i2c_bitrate(0x01U);
}

void i2c_start(void){
    TWCR = (1<<TWINT) | (1<<TWEN) |(1<<TWSTA);
    i2c_wait_for_twint();
}

void i2c_stop(void){
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
    while (! (TWCR & (1<<TWSTO)));
}

void i2c_write(uint8_t dane){
    TWDR = dane;
    TWCR = (1<<TWINT) | (1<<TWEN);
    i2c_wait_for_twint();
}

uint8_t i2c_read(uint8_t ACK){
    if(0 == ACK){
        TWCR = (1<<TWINT) | (1<<TWEN);
    }
    else{
        TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
    }
    i2c_wait_for_twint();
    return TWDR;
}

static void set_i2c_prescaler(i2c_div_t divider){
    uint8_t set_value = 0x0U;
    switch (divider){
    case I2C_DIV1:
        set_value = 0b00U;
        break;
    case I2C_DIV4:
        set_value = 0b01U;
        break;
    case I2C_DIV16:
        set_value = 0b10U;
        break;
    case I2C_DIV64:
        set_value = 0b11U;
        break;
    default:
        set_value = 0b00U;
        break;
    }
    /* clear */
    TWSR &= ~(0b11U << TWPS0);
    /* set */
    TWSR |= (set_value << TWPS0);
}

static void set_i2c_bitrate(uint8_t bitrate){
    TWBR = bitrate;
}

static inline void i2c_wait_for_twint(void){
    while (0 == (TWCR & (1<<TWINT)));
}
