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
    pm->GCCTRL[6].oscsel = 1; /* select oscillator 1 which runs at 12Mhz */
    pm->GCCTRL[6].pllsel = 0; 
    pm->GCCTRL[6].cen = 1;    /* enable the clock */

    /* actually activate the ABDAC */
    dac->CR.en = 1;

    /* activate the interrupt for the ABDAC */
    dac->IER.tx_ready = 1;

    /* connect the interrupt handler to the interrupt signal */
    register_interrupt((__int_handler)handler, IRQ / 32, IRQ % 32, ABDAC_INT_LEVEL);
}
