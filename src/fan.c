#include "include/fan.h"
#include "include/mcu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "include/gpio.h"

struct timer_status {
    /* overflows per duty cycle in high state */
    uint8_t duty_cycle;
};

struct pwm {

    struct timer_status fan_0;
    struct timer_status fan_1;

    uint8_t n_overflows;
    uint8_t prescale;
};

struct pwm fan_pwm;

ISR(TIMER0_OVF_vect){
    fan_pwm.n_overflows++;

    /* start of ducy cycle */
    if(0 == fan_pwm.n_overflows){
        IO_FAN_REG |= (1 << IO_FAN_0) | (1 << IO_FAN_1);
        return;
    }

    /* clear IO if duty cycle reached */
    if(fan_pwm.fan_0.duty_cycle == fan_pwm.n_overflows){
        IO_FAN_REG &= ~(1 << IO_FAN_0);
    }
    if(fan_pwm.fan_1.duty_cycle == fan_pwm.n_overflows){
        IO_FAN_REG &= ~(1 << IO_FAN_1);
    }
}


/* Configure the timers for pwm */
void init_fan(){
    /* make sure the registers are clear */
    TCCR0A = 0x0;
    TCCR0B = 0x0;

    TIMSK0 = 0x01;

    /* Set prescaler to none */
    set_fan_prescaler(FAN_OFF);
}

inline void set_fan_prescaler(fan_div_t prescale){
    uint8_t tmp_TCCR0B = 0x0;
    fan_pwm.prescale = prescale;
    tmp_TCCR0B = TCCR0B;
    tmp_TCCR0B &= ~(0b111 << CS00);
    TCCR0B = tmp_TCCR0B | ((uint8_t)prescale << CS00);
}

void set_fan_dutycycle(fan_ch_t channel, uint8_t duty_cycle){
    switch (channel) {
    case FAN0:
        fan_pwm.fan_0.duty_cycle = duty_cycle;
        break;
    case FAN1:
        fan_pwm.fan_1.duty_cycle = duty_cycle;
        break;
    default:
        /* wrong channel ID given */
        break;
    }
}
