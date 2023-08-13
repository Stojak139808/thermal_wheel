#ifndef SMC_H
#define SMC_H

#include <stdint.h>

/* PWM prescalers */
typedef enum {
    SMC_OFF,
    SMC_NO_PRESCALE = 0b001,
    SMC_DIV8,
    SMC_DIV64,
    SMC_DIV256,
    SMC_DIV1024
}smc_div_t;

void init_smc(void);
void set_smc_prescaler(smc_div_t prescale);
void set_smc_period(uint16_t period);

#endif
