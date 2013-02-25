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

volatile avr32_abdac_t *dac = &AVR32_ABDAC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *pm = &AVR32_PM;

int main(int argc, char *argv[]) {
    init_hardware();

    while(1);
    return 0;
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
    init_interrupts();
}

void init_buttons(void) {
    register_interrupt(button_isr, AVR32_PIOB_IRQ / 32, AVR32_PIOB_IRQ % 32, BUTTONS_INT_LEVEL);
}

void init_audio(void) {

    /* take control of pins 20 and 21 away from PIOB */
    piob->PDR.p20 = 1;
    piob->PDR.p21 = 1;

    /* and give control of the pins to peripheral A (which is the ABDAC) instead */
    piob->ASR.p20 = 1;
    piob->ASR.p21 = 1;

    /* setting up options for the generic clock that will drive the ABDAC */
    pm->GCCTRL[6].oscsel = 0; /* select oscillator 0 which runs at 20Mhz */
    pm->GCCTRL[6].pllsel = 0; /* use oscillators and not PLL */
    pm->GCCTRL[6].cen = 1;    /* enable the clock */
    pm->GCCTRL[6].diven = 1;  /* enable clock speed division to slow the clock */
    pm->GCCTRL[6].div = 226;  /* set the slowing of the clock by 2*(div+1). 226 is
                                 is chosen as a value to give a frequency as close
                                 as possible to 44100Hz */

    /* actually activate the ABDAC */
    dac->CR.en = 1;

    /* activate the interrupt for the ABDAC */
    dac->IER.tx_ready = 1;

    /* connect the interrupt handler to the interrupt signal */
    register_interrupt(abdac_isr, AVR32_ABDAC_IRQ / 32, AVR32_ABDAC_IRQ % 32, ABDAC_INT_LEVEL);
}

void button_isr(void) {
}

void abdac_isr(void) {
    dac->SDR.channel0 = rand();
    dac->SDR.channel1 = rand();
}
