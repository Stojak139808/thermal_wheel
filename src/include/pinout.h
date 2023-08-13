#ifndef PINOUT_H
#define PINOUT_H

#include "gpio.h"

enum pin_direction_enum{
    RESERVED,
    IN = 0,
    OUT
};

#define PORTB_DIR_0 IN
#define PORTB_DIR_1 OUT         /* SMC / OC1A */
#define PORTB_DIR_2 IN
#define PORTB_DIR_3 IN
#define PORTB_DIR_4 IN
#define PORTB_DIR_5 IN
#define PORTB_DIR_6 IN
#define PORTB_DIR_7 IN

#define PORTC_DIR_0 IN
#define PORTC_DIR_1 IN
#define PORTC_DIR_2 IN
#define PORTC_DIR_3 IN
#define PORTC_DIR_4 OUT         /* SDA */
#define PORTC_DIR_5 OUT         /* SCL */
#define PORTC_DIR_6 IN
#define PORTC_DIR_7 RESERVED

#define PORTD_DIR_0 IN          /* RX */
#define PORTD_DIR_1 OUT         /* TX */
#define PORTD_DIR_2 IN
#define PORTD_DIR_3 IN
#define PORTD_DIR_4 IN
#define PORTD_DIR_5 OUT         /* PWM / OC0B */
#define PORTD_DIR_6 OUT         /* PWM / OC0A */
#define PORTD_DIR_7 IN

#define DDRB_COFIG CAST_PINS_TO_UINT8(PORTB_DIR_0, PORTB_DIR_1, PORTB_DIR_2, PORTB_DIR_3, PORTB_DIR_4, PORTB_DIR_5, PORTB_DIR_6, PORTB_DIR_7)
#define DDRC_COFIG CAST_PINS_TO_UINT8(PORTC_DIR_0, PORTC_DIR_1, PORTC_DIR_2, PORTC_DIR_3, PORTC_DIR_4, PORTC_DIR_5, PORTC_DIR_6, PORTC_DIR_7)
#define DDRD_COFIG CAST_PINS_TO_UINT8(PORTD_DIR_0, PORTD_DIR_1, PORTD_DIR_2, PORTD_DIR_3, PORTD_DIR_4, PORTD_DIR_5, PORTD_DIR_6, PORTD_DIR_7)

#endif
