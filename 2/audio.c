#include <sys/interrupts.h>
#include <avr32/ap7000.h>
#include "audio.h"

void audio_init(volatile avr32_pio_t* pio, volatile avr32_pm_t* pm, volatile avr32_abdac_t* dac, void* handler, int IRQ) {

    /* take control of pins 20 and 21 away from PIOB */
    pio->PDR.p20 = 1;
    pio->PDR.p21 = 1;

    /* and give control of the pins to peripheral A (which is the ABDAC) instead */
    pio->ASR.p20 = 1;
    pio->ASR.p21 = 1;

    /* setting up options for the generic clock that will drive the ABDAC */
    pm->GCCTRL[6].oscsel = 1; /* select oscillator 0 which runs at 20Mhz */
    pm->GCCTRL[6].pllsel = 0; /* use oscillators and not PLL */
    pm->GCCTRL[6].cen = 1;    /* enable the clock */
    pm->GCCTRL[6].diven = 0;  /* enable clock speed division to slow the clock */
    pm->GCCTRL[6].div = 0*226;  /* set the slowing of the clock by 2*(div+1). 226 is
                                 is chosen as a value to give a frequency as close
                                 as possible to 44100Hz */

    /* actually activate the ABDAC */
    dac->CR.en = 1;

    /* activate the interrupt for the ABDAC */
    dac->IER.tx_ready = 1;

    /* connect the interrupt handler to the interrupt signal */
    register_interrupt((__int_handler)handler, IRQ / 32, IRQ % 32, ABDAC_INT_LEVEL);
}
