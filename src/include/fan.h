#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/* PWM channels */
typedef enum {
    FAN0,
    FAN1
}fan_ch_t;

/* PWM prescalers */
typedef enum {
    FAN_OFF,
    FAN_NO_PRESCALE = 0b001,
    FAN_DIV8,
    FAN_DIV64,
    FAN_DIV256,
    FAN_DIV1024
}fan_div_t;

void init_fan(void);
void set_fan_dutycycle(fan_ch_t channel, uint8_t duty_cycle);
void set_fan_prescaler(fan_div_t prescale);


#endif
