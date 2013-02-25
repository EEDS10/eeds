/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include <avr32/ap7000.h>
#include <sys/interrupts.h>
#include <stdlib.h>
#include "oeving2.h"
#include "leds.h"
#include "buttons.h"
#include "audio.h"

volatile avr32_abdac_t *dac = &AVR32_ABDAC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *pm = &AVR32_PM;

int main(int argc, char *argv[]) {
    init_hardware();

    while(1);

    return 0;
}


/* function that uses the leds to count to some value */
void LEDcounting(int countTo) {
    int derp = 0;
    while(countTo > 0) {
        countTo = countTo - 1;
        /* delay for a bit */
        int k = 100000;
        while(k > 0) {k = k-1;}
        /* turn previous leds off, turn next leds on */
        leds_off(derp);
        derp = (derp++ % countTo);
        leds_on(derp);
    }
}

/* sets up the hardware */
void init_hardware(void) {
    leds_init(pioc, 0xff);
    buttons_init(piob, 0xff, button_isr, AVR32_PIOB_IRQ);
    audio_init(piob, pm, dac, abdac_isr, AVR32_ABDAC_IRQ);
    init_intc();
}

void init_intc(void) {
    /* as specified on page 45 of the compendium,
     * the arguments of set_interrupts_base will always be
     * (void *)AVR32_INTC_ADDRESS */
    set_interrupts_base((void *)AVR32_INTC_ADDRESS);
    /* init_interrupts() is contained in something
     * which apparently the Makefile incorporates into this
     * without us even having asked for it. Thanks! */
    init_interrupts();
}


void button_isr(void) {
    piob->isr;
    leds_off(0xff);
    leds_on(buttons_read());
}


void abdac_isr(void) {
    dac->SDR.channel0 = rand();
    dac->SDR.channel1 = rand();
}
