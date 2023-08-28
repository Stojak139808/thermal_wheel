#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>

typedef enum {
    SYS_OFF,
    SYS_NO_PRESCALE = 0b001,
    SYS_DIV8,
    SYS_DIV32,
    SYS_DIV64,
    SYS_DIV128,
    SYS_DIV256,
    SYS_DIV1024
}sysclock_div_t;

typedef void (*sys_callback_t)(void);

void init_control();
void set_syscallback(sys_callback_t new_callback);
void set_syscounter(uint16_t tick_count, sysclock_div_t prescaler);
uint32_t get_syscounter(void);

#endif
