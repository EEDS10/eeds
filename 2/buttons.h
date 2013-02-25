#ifndef BUTTONS_H
#define BUTTONS_H

#include <avr32/ap7000.h>

#define BUTTONS_INT_LEVEL 0

void buttons_init(volatile avr32_pio_t *pio, int bitmask, void *handler, int IRQ);
int buttons_read(void);

#endif
