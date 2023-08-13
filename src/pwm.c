#include "include/pwm.h"
#include "include/mcu.h"
#include <avr/io.h>

/* Configure the timers for pwm */
void init_pwm(){
    /* make sure the registers are clear */
    TCCR0A = 0x0;
    TCCR0B = 0x0;

    /* OC0A inverting mode, Fast PWM mode */
    TCCR0A |= (0b10 << COM0A0) | (1 << WGM00) | (1 << WGM01) | (0b11 << COM0B0);

    /* Set prescaler to none */
    TCCR0B |= 0b001 << CS00;
}

inline void set_pwm_prescaler(pwm_div_t prescale){
    uint8_t tmp_TCCR0B = TCCR0B;
    tmp_TCCR0B &= ~(0b111 << CS00);
    TCCR0B = tmp_TCCR0B | ((uint8_t)prescale << CS00);
}

void set_pwm_dutycycle(pwm_ch_t channel, uint8_t duty_cycle){
    switch (channel) {
    case PWM0:
        OCR0A = duty_cycle;
        break;
    case PWM1:
        OCR0B = duty_cycle;
        break;
    default:
        /* wrong channel ID given */
        break;
    }
}
