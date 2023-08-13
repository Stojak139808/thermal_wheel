#include "include/pinout.h"
#include "include/mcu.h"
#include <avr/io.h>

void setup_gpio(){
    DDRB = DDRB_COFIG;
    DDRC = DDRC_COFIG;
    DDRD = DDRD_COFIG;
    PORTB = 0x0;
    PORTC = 0x0;
    PORTD = 0x0;
}
