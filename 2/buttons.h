#ifndef BUTTONS_H
#define BUTTONS_H

#include <avr32/ap7000.h>

#define BUTTONS_INT_LEVEL 0

#define BUTTON_0 0x1
#define BUTTON_1 0x2
#define BUTTON_2 0x4
#define BUTTON_3 0x8
#define BUTTON_4 0x10
#define BUTTON_5 0x20
#define BUTTON_6 0x40
#define BUTTON_7 0x80

void buttons_init(volatile avr32_pio_t *pio, int bitmask, void *handler, int IRQ);
int buttons_read(void);

#endif
