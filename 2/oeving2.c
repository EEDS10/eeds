/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include <avr32/ap7000.h>
#include <sys/interrupts.h>
#include "oeving2.h"
#include "leds.h"

volatile avr32_abdac_t *dac = &AVR32_ABDAC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_smc_t *sm = &AVR32_SMC;

int main(int argc, char *argv[]) {
    init_hardware();

    leds_off(0xff);
    leds_on(0xaa);
    leds_off(0xff);

    int derp = 0x00;
    while(1) {
    }
    return 0;
}

/* function that uses the leds to count to some value */
void LEDcounting(int countTo) {
    while(countTo > 0) {
        countTo = countTo - 1;
        /* delay for a bit */
        int k = 100000;
        while(k > 0) {k = k-1;}
        /* turn previous leds off, turn next leds on */
        leds_off(derp);
        derp = (derp++ % 0xff);
        leds_on(derp);
    }
}
/* funksjon for å initialisere maskinvaren, må utvides */
void init_hardware(void) {
    init_intc();
    leds_init(pioc, 0xff);
    init_buttons();
    init_audio();
}

void init_intc(void) {
    set_interrupts_base((void *)AVR32_INTC_ADDRESS);
    /* init_interrupts() is contained in something
     * which apparently the Makefile incorporates into this
     * without us even having asked for it. Thanks! */
    init_interrupts();
}

void init_buttons(void) {
    register_interrupt(button_isr, AVR32_PIOB_IRQ / 32, AVR32_PIOB_IRQ % 32, BUTTONS_INT_LEVEL);
    /* (...) */
    /* Enable I/O pins for them buttans */
    piob->per = 0xff;
    /* Enable pull-up resistors */
    piob->puer = 0xff;
}

void init_audio(void) {
    register_interrupt(abdac_isr, AVR32_ABDAC_IRQ / 32, AVR32_ABDAC_IRQ % 32, ABDAC_INT_LEVEL);
    /* (...) */
}

void button_isr(void) {
}

void abdac_isr(void) {
}
