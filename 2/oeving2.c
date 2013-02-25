/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include <avr32/ap7000.h>
#include <sys/interrupts.h>
#include "oeving2.h"

volatile avr32_abdac_t *dac = &AVR32_ABDAC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_smc_t *sm = &AVR32_SMC;

int main(int argc, char *argv[]) {
  init_hardware();

  while(1){
    pioc->sodr = 0xff;
  };
  return 0;
}

/* funksjon for å initialisere maskinvaren, må utvides */
void init_hardware(void) {
  init_intc();
  init_leds();
  init_buttons();
  init_audio();
}

void init_intc(void) {
  set_interrupts_base((void *)AVR32_INTC_ADDRESS);
  init_interrupts();
}

void init_buttons(void) {
  register_interrupt(button_isr, AVR32_PIOB_IRQ / 32, AVR32_PIOB_IRQ % 32, BUTTONS_INT_LEVEL);
  /* (...) */
}

void init_leds(void) {
    pioc->per = 0xff;
    pioc->oer = 0xff;
}

void init_audio(void) {
  register_interrupt(abdac_isr, AVR32_ABDAC_IRQ / 32, AVR32_ABDAC_IRQ % 32, ABDAC_INT_LEVEL);
  /* (...) */
}

void button_isr(void) {
}

void abdac_isr(void) {
}
