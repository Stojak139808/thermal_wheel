#include "include/control.h"
#include "include/mcu.h"
#include "include/bmp280.h"
#include <avr/io.h>
#include <avr/interrupt.h>

struct timer_status {

    /* number of overflows */
    uint8_t overflow_counter;

    /* clk ticks per callback */
    uint16_t tickcount;

    /* prescaler */
    uint8_t prescaler;
};

struct control_module {

    /* lifetime counter for timestamps */
    uint32_t sys_counter;

    /* callback with processes to perform on timer trigger */
    sys_callback_t callback;

    /* configuration for the system timer */
    struct timer_status timer_config;
};

typedef struct control_module control_module_t;

control_module_t config = {.callback = (sys_callback_t)0x0};

static void update_syscounter(uint16_t tick_count, uint8_t prescale);

ISR(TIMER2_OVF_vect){
    /* count number of overflows */
    config.timer_config.overflow_counter++;
}

ISR(TIMER2_COMPA_vect){
    /* check number of overflows */
    if(config.timer_config.overflow_counter <
       (config.timer_config.tickcount & 0xFF00U) >> 8){
        return;
    }

    /* unlock other interrupts, so motors spin smoothly */
    sei();

    config.timer_config.overflow_counter = 0;
    config.sys_counter++;

    /* call main callback */
    if (0 != config.callback){
        config.callback();
    }
}

static void update_syscounter(uint16_t tick_count, uint8_t prescale){
    OCR2A = (uint8_t)(tick_count & 0x00FFU);
    TCCR2B = (prescale & 0b111) << CS20;
}

void init_control(){
    config.sys_counter = 0;
    update_syscounter(config.timer_config.tickcount,
                      config.timer_config.prescaler);
    TIMSK2 = 0b011; /* enable interrupt on sysclock */
}

void set_syscallback(sys_callback_t new_callback){
    config.callback = new_callback;
}

void set_syscounter(uint16_t tick_count, sysclock_div_t prescaler){
    config.timer_config.tickcount = tick_count;
    config.timer_config.prescaler = (uint8_t)prescaler & 0b111;
    update_syscounter(config.timer_config.tickcount,
                      config.timer_config.prescaler);
}

uint32_t get_syscounter(){
    uint32_t ret;
    cli();
    ret = config.sys_counter;
    sei();
    return ret;
}
