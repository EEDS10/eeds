#include <avr32/ap7000.h>
#include "leds.h"

static volatile avr32_pio_t *pio;

void leds_init(volatile avr32_pio_t* _pio, int bitfield) {

    /* Store a reference to the PIO which has the leds */
    pio = _pio;

    /* Enable IO pins */
    pio->per = bitfield;

    /* Set pins to act as output */
    pio->oer = bitfield;
}

void leds_on(int bitfield){
    
    /* Turn on leds */
    pio->sodr = bitfield;
}

void leds_off(int bitfield){

    /* Turn off leds */
    pio->codr = bitfield;
}
