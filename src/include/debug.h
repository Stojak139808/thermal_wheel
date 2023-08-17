#ifndef DEBUG_H
#define DEBUG_H

/* debbuging macros, only included if DEBUG_BUILD is passed */

#include <stdio.h>
#include <stdint.h>
#include "uart.h"

#define DEBUG_BUFF_SIZE 64

#ifdef DEBUG_BUILD
    #define DEBUG_TEST 1
    extern uint8_t debug_buffer[DEBUG_BUFF_SIZE];
#else
    #define DEBUG_TEST 0
    /* this variable gets optimized away */
    extern uint8_t *debug_buffer;
#endif

#define DEBUG(fmt, ...) \
    do { \
        if( DEBUG_TEST ){ \
            sprintf(debug_buffer, fmt, ##__VA_ARGS__); \
            send_uart(debug_buffer, SEND_FORCE); \
        } \
    } while (0)

#endif