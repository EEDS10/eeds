#include <sys/interrupts.h>
#include <avr32/ap7000.h>
#include "buttons.h"
#include "oeving2.h"

void buttons_init(volatile avr32_pio_t *pio, int bitmask, void *handler, int IRQ) {
    register_interrupt((__int_handler)(handler), IRQ / 32, IRQ % 32, BUTTONS_INT_LEVEL);

    /* Enable I/O pins for them buttans */
    pio->per = bitmask;

    /* Enable pull-up resistors */
    pio->puer = bitmask;
}
