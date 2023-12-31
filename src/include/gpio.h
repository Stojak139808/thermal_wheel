#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "pinout.h"

void init_gpio();

#define CAST_PINS_TO_UINT8(p0, p1, p2, p3, p4, p5, p6, p7)  \
    (uint8_t)(p0 | \
    (p1 << 1) | \
    (p2 << 2) | \
    (p3 << 3) | \
    (p4 << 4) | \
    (p5 << 5) | \
    (p6 << 6) | \
    (p7 << 7))

#endif
