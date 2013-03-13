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
#include "synth.h"

volatile avr32_abdac_t *dac = &AVR32_ABDAC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *pm = &AVR32_PM;

MOD* MODS[MODFILES_N];
MOD_Player* player;

int current_selection = 0;

int ticks = 0;
int debticker = 0;

int deb = 0xaf;
int previous_out = 0;

sound_t soundA, soundB;
playback_t playbackA, playbackB;
playback_t *current_synth_sound = NULL;

#define SAMPLE_RATE (46875/2)

int main(int argc, char *argv[]) {

    prepare_playback(&playbackA, get_sound(&soundA, SQUARE, 500, 500, 1250, 0, 250, 20), SAMPLE_RATE, 20000);
    prepare_playback(&playbackB, get_sound(&soundB, SAWTOOTH, 2200, 100, 100, 0, 500, 70), SAMPLE_RATE, 20000);

    MODS[0] = MOD_load(MODFILES_BACONGRYTOR_MOD);
    MODS[1] = MOD_load(MODFILES_HOFFMAN___DROP_THE_PANIC__TWEAKED__MOD);
    MODS[2] = MOD_load(MODFILES_BANANASPLIT_MOD);
    MODS[3] = MOD_load(MODFILES_TUULENVIRE_MOD);

    player = MOD_Player_create(SAMPLE_RATE);

    init_hardware();

    leds_off(0xff);
    leds_on(0xff);


    while(1){
        while(ticks > 0){
            if(player->mod != NULL){
                ticks-=16;
                MOD_Player_step(player, 1000000/SAMPLE_RATE*16);
            }
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
        current_synth_sound = NULL;
    }

    if(buttons == 0x2){
        current_selection = 1;
        MOD_Player_set_mod(player, MODS[current_selection]);
        current_synth_sound = NULL;
    }

    if(buttons == 0x4){
        current_selection = 2;
        MOD_Player_set_mod(player, MODS[current_selection]);
        current_synth_sound = NULL;
    }

    if(buttons == 0x8){
        current_selection = 3;
        MOD_Player_set_mod(player, MODS[current_selection]);
        current_synth_sound = NULL;
    }

    if(buttons == 0x10){
        current_selection = 4;
        MOD_Player_set_mod(player, NULL);
        current_synth_sound = &playbackA;
        reset_playback(&playbackA);
    }

    if(buttons == 0x20){
        current_selection = 5;
        MOD_Player_set_mod(player, NULL);
        current_synth_sound = &playbackB;
        reset_playback(&playbackB);
    }

    leds_off(0xff);
    if(current_selection != -1){
        leds_on(1<<current_selection);
    }
}


void abdac_isr(void) {
    int32_t out = 0;

    /* Play mod files */
    if(player->mod != NULL){
        out += MOD_Player_play(player);
        ticks++;
    }

    if(current_synth_sound != NULL){
        out += next_sample(current_synth_sound);
    }

    dac->SDR.channel0 = out;
    dac->SDR.channel1 = out;
}
