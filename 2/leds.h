#ifndef LEDS_H
#define LEDS_H

#include <avr32/ap7000.h>

void leds_init(volatile avr32_pio_t* pio, int bitfield);
void leds_on(int bitfield);
void leds_off(int bitfield);


#endif
