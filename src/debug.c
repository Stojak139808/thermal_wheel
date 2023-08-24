#include "include/debug.h"

#ifdef DEBUG_BUILD
    #define DEBUG_TEST 1
    uint8_t debug_buffer[DEBUG_BUFF_SIZE] = { [0 ... DEBUG_BUFF_SIZE-1] = 0x00};
#else
    #define DEBUG_TEST 0
    /* this variable gets optimized away */
    uint8_t *debug_buffer;
#endif
