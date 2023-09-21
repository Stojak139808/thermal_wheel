#include "include/mcu.h"
#include <avr/io.h>
#include "include/smc.h"
#include <avr/interrupt.h>
#include "include/pinout.h"

void init_smc(){
    TCCR1A = 0x0;
    TCCR1B = 0x0;

    /* set OC1A to toggle mode */
    TCCR1A |= (0b01 << COM1B0);

    /* set CTC mode */
    TCCR1A &= ~(0b00 << WGM10);
    TCCR1B |= (0b01 << WGM12);

    /* set SMC to off */
    set_smc_prescaler(SMC_OFF);
    set_smc_direction(0);
}

void set_smc_prescaler(smc_div_t prescale){
    uint8_t tmp_TCCR1B = TCCR1B;
    /* clear old prescale */
    tmp_TCCR1B &= ~(0b111 << CS00);
    TCCR1B = tmp_TCCR1B | ((uint8_t)prescale << CS00);
}

void set_smc_period(uint16_t period){
    cli();
    OCR1AH = (uint8_t)((period & 0xff00) >> 8);
    OCR1AL = (uint8_t)(period & 0x00ff);
    OCR1BH = (uint8_t)((period & 0xff00) >> 8);
    OCR1BL = (uint8_t)(period & 0x00ff);
    sei();
}

void set_smc_direction(uint8_t direction){
    if(0 != direction){
        IO_SMC_REG |= (1 << IO_SMC_DIR);
    }
    else{
        IO_SMC_REG &= ~(1 << IO_SMC_DIR);
    }
}
