#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/* PWM channels */
typedef enum {
    PWM0,
    PWM1
}pwm_ch_t;

/* PWM prescalers */
typedef enum {
    PWM_OFF,
    NO_PRESCALE = 0b001,
    DIV8,
    DIV64,
    DIV256,
    DIV1024
}pwm_div_t;

void init_pwm(void);
void set_pwm_dutycycle(pwm_ch_t channel, uint8_t duty_cycle);
void set_pwm_prescaler(pwm_div_t prescale);


#endif