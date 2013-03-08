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

MOD* MODS[MODFILES_N];
MOD_Player* player;

int current_selection = -1;

int ticks = 0;
int debticker = 0;

int deb = 0xaf;
int previous_out = 0;

#define SAMPLE_RATE (46875)


int main(int argc, char *argv[]) {

    {int i; for(i=0;i<MODFILES_N;i++){
        MODS[i] = MOD_load(MODFILES[i]);    
    }}

    /*
    {int i; MOD* mod = MOD_load(MODFILES_BACONGRYTOR_MOD);
        for(i=0;i<MODFILES_N;i++){
        MODS[i] = mod;
    }}
    */

    player = MOD_Player_create(SAMPLE_RATE/2);

    MOD_Player_set_mod(player, MODS[0]);

    init_hardware();

    leds_off(0xff);

    while(1){
        while(ticks){
            ticks--;
            MOD_Player_step(player, 2000000/SAMPLE_RATE + 1);
        }
    }

    return 0;
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

    int buttons = buttons_read();

    if(buttons == 0x1){
        current_selection = 0;
        MOD_Player_set_mod(player, MODS[current_selection]);
    }

    if(buttons == 0x2){
        current_selection = 1;
        MOD_Player_set_mod(player, MODS[current_selection]);
    }

    if(buttons == 0x4){
        current_selection = 2;
        MOD_Player_set_mod(player, MODS[current_selection]);
    }

    if(buttons == 0x8){
        current_selection = 3;
        MOD_Player_set_mod(player, MODS[current_selection]);
    }

    leds_off(0xff);
    if(current_selection != -1){
        leds_on(1<<current_selection);
    }
}


void abdac_isr(void) {

    int16_t out = MOD_Player_play(player);

    dac->SDR.channel0 = out;
    dac->SDR.channel1 = out;

    ticks++;
}
