#include "include/mcu.h"
#include <avr/io.h>
#include "include/smc.h"
#include <avr/interrupt.h>
#include "include/gpio.h"

struct timer_status {

    /* number of overflows */
    uint8_t overflow_counter;

    /* clk ticks per callback */
    uint32_t tickcount;

    /* prescaler */
    uint16_t prescaler;
};

struct timer_status smc_config;


/* ISR's for software PWM */
ISR(TIMER1_OVF_vect){
    smc_config.overflow_counter++;
}

ISR(TIMER1_COMPA_vect){
    if(smc_config.overflow_counter <
       (uint8_t)((smc_config.tickcount & 0x00FF0000 ) >> 16)){
        return;
    }
    smc_config.overflow_counter = 0;

    /* toggle motor STEP GPIO */
    IO_SMC_REG ^= (1 << IO_SMC_STEP);
}

void init_smc(){
    TCCR1A = 0x0;
    TCCR1B = 0x0;

    TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);

    /* set SMC to off */
    set_smc_prescaler(SMC_OFF);

    set_smc_direction(0);

    /* set enable pin to 1 */
    IO_SMC_REG |= (1 << IO_SMC_EN);
}

void set_smc_prescaler(smc_div_t prescale){
    uint8_t tmp_TCCR1B = TCCR1B;
    /* clear old prescale */
    tmp_TCCR1B &= ~(0b111 << CS00);
    TCCR1B = tmp_TCCR1B | ((uint8_t)prescale << CS00);
}


void set_smc_period(uint32_t period){

    /* cap the value to 24bit */
    if(period > 0x00FFFFFFU){
        period = 0x00FFFFFFU;
    }
    smc_config.tickcount = period;
    cli();
    OCR1AH = (uint8_t)((period & 0xff00) >> 8);
    OCR1AL = (uint8_t)(period & 0x00ff);
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
