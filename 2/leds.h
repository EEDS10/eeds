#ifndef LEDS_H
#define LEDS_H

#include <avr32/ap7000.h>

#define LED_0 0x1
#define LED_1 0x2
#define LED_2 0x4
#define LED_3 0x8
#define LED_4 0x10
#define LED_5 0x20
#define LED_6 0x40
#define LED_7 0x80

void leds_init(volatile avr32_pio_t* pio, int bitfield);
void leds_on(int bitfield);
void leds_off(int bitfield);


#endif
