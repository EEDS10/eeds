#include <sys/interrupts.h>
#include <avr32/ap7000.h>
#include "buttons.h"

static volatile avr32_pio_t *pio;

static int previous_button_state = 0;

void buttons_init(volatile avr32_pio_t *_pio, int bitmask, void *handler, int IRQ) {

    /* Store a reference to the PIO which has the leds */
    pio = _pio;

    /* Enable I/O pins for them buttans */
    pio->per = bitmask;

    /* Enable pull-up resistors */
    pio->puer = bitmask;

    /* enable interrupts */
    pio->ier = bitmask;

    /* connect the interrupt handler to the interrupt signal */
    register_interrupt(handler, IRQ / 32, IRQ % 32, BUTTONS_INT_LEVEL);
}

int buttons_read(){
    int buttons = pio->pdsr;
    previous_button_state |= buttons;
    previous_button_state = ~previous_button_state;
    return previous_button_state&0xff;
}
