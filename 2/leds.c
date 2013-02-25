#include <avr32/ap7000.h>
#include "leds.h"

static volatile avr32_pio_t *pio;

void leds_init(volatile avr32_pio_t* _pio, int bitfield) {

    /* sore a reference to the PIO which has the leds */
    pio = _pio;

    /* enable IO pins */
    pio->per = bitfield;

    /* set pins to act as output */
    pio->oer = bitfield;
}

void leds_on(int bitfield){
    
    /* turn on leds */
    pio->sodr = bitfield;
}

void leds_off(int bitfield){

    /* turn off leds */
    pio->codr = bitfield;
}
