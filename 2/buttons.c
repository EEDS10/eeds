#include <sys/interrupts.h>
#include <avr32/ap7000.h>
#include "buttons.h"
#include "oeving2.h"

static volatile avr32_pio_t *pio;

void buttons_init(volatile avr32_pio_t *_pio, int bitmask, void *handler, int IRQ) {

    /* Store a reference to the PIO which has the leds */
    pio = _pio;

    /* Enable I/O pins for them buttans */
    pio->per = bitmask;

    /* Enable pull-up resistors */
    pio->puer = bitmask;

    /* connect the interrupt handler to the interrupt signal */
    register_interrupt((__int_handler)(handler), IRQ / 32, IRQ % 32, BUTTONS_INT_LEVEL);
}

int buttons_read(){
    return ~pio->pdsr;
}
