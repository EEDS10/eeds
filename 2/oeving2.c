/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include <avr32/ap7000.h>
#include <sys/interrupts.h>
#include <stdlib.h>
#include <stdint.h>
#include "oeving2.h"
#include "leds.h"
#include "buttons.h"
#include "audio.h"
#include "MOD.h"
#include "Player.h"
#include "modfiles.h"

volatile avr32_abdac_t *dac = &AVR32_ABDAC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *pm = &AVR32_PM;

#define BUFSIZE 4096

MOD* MODS[MODFILES_N];
MOD* mod;
MOD_Player* player;
int counter = 0;
int16_t buffer[BUFSIZE];

int current_selection = 0;

int16_t out = 0;

int main(int argc, char *argv[]) {


    {int i; for(i=0;i<MODFILES_N;i++){
        MODS[i] = MOD_load(MODFILES[i]);    
    }}

    /*
    MODS[0] = MOD_load(MODFILES[0]);
    */

    mod = MODS[0];

    player = MOD_Player_create();

    MOD_Player_set_mod(player, mod);

    init_hardware();

    leds_off(0xff);

    if(player && player->channels[0] && player->channels[1] && player->channels[2] && player->channels[3]){
        leds_on(0x80);
    }

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

    int buttons = buttons_read();


    if(buttons == 0x1){
        current_selection = (current_selection + 1) % MODFILES_N;
        mod = MODS[current_selection];
        MOD_Player_set_mod(player, mod);
    }

    if(buttons == 0x2){
        current_selection = (current_selection + MODFILES_N - 1) % MODFILES_N;
        mod = MODS[current_selection];
        MOD_Player_set_mod(player, mod);
    }

    leds_on(current_selection);
}


void abdac_isr(void) {
    out = MOD_Player_play(player, mod);
    dac->SDR.channel0 = out;
    dac->SDR.channel1 = out;

}
