#include "include/pinout.h"
#include "include/mcu.h"
#include <avr/io.h>

void init_gpio(){
    DDRB = DDRB_COFIG;
    DDRC = DDRC_COFIG;
    DDRD = DDRD_COFIG;
    PORTB = 0x00u;
    PORTC = 0x00u;
    PORTD = 0x00u;
}
