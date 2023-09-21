#ifndef PINOUT_H
#define PINOUT_H

#include "mcu.h"
#include <avr/io.h>
#include "gpio.h"


enum pin_direction_enum{
    RESERVED,
    IN = 0,
    OUT
};

#define IO_SMC_STEP PINB2
#define IO_SMC_DIR  PINB5
#define IO_SMC_EN   PINB1
#define IO_SMC_REG  PORTB

#define IO_FAN_0    PORTD4
#define IO_FAN_1    PORTD5
#define IO_FAN_REG  PORTD

#define PORTB_DIR_0 IN          /* Y DIR */
#define PORTB_DIR_1 OUT         /* X EN  */
#define PORTB_DIR_2 OUT         /* X CUR / new step */
#define PORTB_DIR_3 IN          /* Y CUR */
#define PORTB_DIR_4 IN          /* X STEP / old step */
#define PORTB_DIR_5 OUT         /* X DIR */
#define PORTB_DIR_6 RESERVED
#define PORTB_DIR_7 RESERVED

#define PORTC_DIR_0 IN
#define PORTC_DIR_1 IN
#define PORTC_DIR_2 IN
#define PORTC_DIR_3 IN
#define PORTC_DIR_4 IN          /* SDA */
#define PORTC_DIR_5 IN          /* SCL */
#define PORTC_DIR_6 IN
#define PORTC_DIR_7 RESERVED

#define PORTD_DIR_0 IN          /* RX */
#define PORTD_DIR_1 OUT         /* TX */
#define PORTD_DIR_2 IN          /* LASER 1 */
#define PORTD_DIR_3 IN          /* LASER 2 */
#define PORTD_DIR_4 OUT         /* LASER 3 */
#define PORTD_DIR_5 OUT         /* LASER 4 */
#define PORTD_DIR_6 IN          /* Y EN / PWM / OC0A */
#define PORTD_DIR_7 IN          /* Y STEP */

#define DDRB_COFIG CAST_PINS_TO_UINT8(PORTB_DIR_0, PORTB_DIR_1, PORTB_DIR_2, \
                                      PORTB_DIR_3, PORTB_DIR_4, PORTB_DIR_5, \
                                      PORTB_DIR_6, PORTB_DIR_7)
#define DDRC_COFIG CAST_PINS_TO_UINT8(PORTC_DIR_0, PORTC_DIR_1, PORTC_DIR_2, \
                                      PORTC_DIR_3, PORTC_DIR_4, PORTC_DIR_5, \
                                      PORTC_DIR_6, PORTC_DIR_7)
#define DDRD_COFIG CAST_PINS_TO_UINT8(PORTD_DIR_0, PORTD_DIR_1, PORTD_DIR_2, \
                                      PORTD_DIR_3, PORTD_DIR_4, PORTD_DIR_5, \
                                      PORTD_DIR_6, PORTD_DIR_7)

#endif
