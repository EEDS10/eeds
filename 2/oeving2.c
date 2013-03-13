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

MOD* MODS[4];
MOD_Player* player;

int current_selection = 7;
int ticks = 0;

sound_t SOUNDS[3];
playback_t SOUND_PLAYBACKS[3];
playback_t *current_synth_sound = NULL;

#define SAMPLE_RATE (46875/2)

int main(int argc, char *argv[]) {

    prepare_playback(&SOUND_PLAYBACKS[0], get_sound(&SOUNDS[0], SQUARE, 500, 500, 1250, 0, 250, 20), SAMPLE_RATE, 20000);
    prepare_playback(&SOUND_PLAYBACKS[1], get_sound(&SOUNDS[1], SAWTOOTH, 2200, 100, 100, 0, 500, 70), SAMPLE_RATE, 20000);
    prepare_playback(&SOUND_PLAYBACKS[2], get_sound(&SOUNDS[2], NOISE, 1, 0, 1000, 0, 0, 0), SAMPLE_RATE, 20000);

    MODS[0] = MOD_load(MODFILES_BACONGRYTOR_MOD);
    MODS[1] = MOD_load(MODFILES_HOFFMAN___DROP_THE_PANIC__TWEAKED__MOD);
    MODS[2] = MOD_load(MODFILES_BOESENDORFER_P_S_S_MOD);
    MODS[3] = MOD_load(MODFILES_TUULENVIRE_MOD);

    player = MOD_Player_create(SAMPLE_RATE);

    init_hardware();

    leds_off(0xff);


    while(1){
        while(ticks > 0){
            if(player->mod != NULL){
                ticks-=16;
                MOD_Player_step(player, 1000000/SAMPLE_RATE*16);
            }
        }
        if(current_synth_sound != NULL && playback_finished(current_synth_sound)){
            select(7);
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


void select(int selection){
    if(7 == selection){
        MOD_Player_set_mod(player, NULL);
        current_synth_sound = NULL;
    }else if(selection < 4){
        MOD_Player_set_mod(player, MODS[selection]);
        current_synth_sound = NULL;
    }else if(selection < 7){
        MOD_Player_set_mod(player, NULL);
        current_synth_sound = &SOUND_PLAYBACKS[selection - 4];
        reset_playback(current_synth_sound);
    }

    current_selection = selection;

    leds_off(0xff);
    if(current_selection != 7){
        leds_on(1<<current_selection);
    }
}


void button_isr(void) {
    piob->isr;

    int buttons = buttons_read();

    if(!buttons) return;

    int selected = 0;
    while(buttons = buttons>>1){
        selected++;
    }

    select(selected);
}


void abdac_isr(void) {
    int32_t out = 0;

    /* Play mod files */
    if(player->mod != NULL){
        out += MOD_Player_play(player);
        ticks++;
    }

    /* Play sound effects */
    if(current_synth_sound != NULL){
        out += next_sample(current_synth_sound);
    }

    dac->SDR.channel0 = out;
    dac->SDR.channel1 = out;
}
