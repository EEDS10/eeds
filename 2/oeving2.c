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

#define SAMPLE_RATE (46875/2)

#define SELECTION_NONE 7
#define N_MODS 4
#define N_SOUNDS 3

/* hardware handles */
volatile avr32_abdac_t *dac = &AVR32_ABDAC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *pm = &AVR32_PM;

/* variables used for MOD playback */
MOD* MODS[N_MODS];
MOD_Player* player;
int mod_ticks = 0;

/* variables used for sound effect playback */
sound_t SOUNDS[N_SOUNDS];
playback_t SOUND_PLAYBACKS[N_SOUNDS];
playback_t *current_synth_sound = NULL;

/* variables used for keeping track of the user interface state */
int current_selection = SELECTION_NONE;


int main(int argc, char *argv[]) {

    /* perpare sound effects for playback */
    prepare_playback(&SOUND_PLAYBACKS[0], get_sound(&SOUNDS[0], SQUARE, 500, 500, 1250, 0, 250, 20), SAMPLE_RATE, 20000);
    prepare_playback(&SOUND_PLAYBACKS[1], get_sound(&SOUNDS[1], SAWTOOTH, 2200, 100, 100, 0, 500, 70), SAMPLE_RATE, 20000);
    prepare_playback(&SOUND_PLAYBACKS[2], get_sound(&SOUNDS[2], NOISE, 1, 0, 1000, 0, 0, 0), SAMPLE_RATE, 20000);

    /* prepare MOD files for playback */
    MODS[0] = MOD_load(MODFILES_BACONGRYTOR_MOD);
    MODS[1] = MOD_load(MODFILES_HOFFMAN___DROP_THE_PANIC__TWEAKED__MOD);
    MODS[2] = MOD_load(MODFILES_BOESENDORFER_P_S_S_MOD);
    MODS[3] = MOD_load(MODFILES_TUULENVIRE_MOD);
    player = MOD_Player_create(SAMPLE_RATE);

    /* Activate leds, buttons, audio. This is done as late as
     * possible in the setup phase to avoid premature interrupts. */
    init_hardware();

    leds_off(0xff);

    /* main loop */
    while(1){

        /* if we need to logical advance the mod player, do so */
        while(mod_ticks > 0){
            if(player->mod != NULL){
                mod_ticks-=32;
                MOD_Player_step(player, 32*1000000/SAMPLE_RATE);
            }
        }

        /* if we are done playing a sound effect, mark it as done */
        if(current_synth_sound != NULL && playback_finished(current_synth_sound)){
            select(SELECTION_NONE);
        }
    }

    /* Return success. This is ironic, because if the program ever
     * gets this far, something must have gone wrong :D */
    return EXIT_SUCCESS;
}


/* sets up the hardware */
void init_hardware(void) {
    leds_init(pioc, 0xff);
    buttons_init(piob, 0xff, button_isr, AVR32_PIOB_IRQ);
    audio_init(piob, pm, dac, abdac_isr, AVR32_ABDAC_IRQ);
    init_intc();
}


/* sets up interrupts */
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


/* selects a MOD file or sound effect to play */
void select(int selection){

    /* set the correct sound to play */
    if(SELECTION_NONE == selection){
        MOD_Player_set_mod(player, NULL);
        current_synth_sound = NULL;
    }else if(selection < N_MODS){
        MOD_Player_set_mod(player, MODS[selection]);
        current_synth_sound = NULL;
    }else {
        MOD_Player_set_mod(player, NULL);
        current_synth_sound = &SOUND_PLAYBACKS[selection - N_MODS];
        reset_playback(current_synth_sound);
    }

    /* update state variables */
    current_selection = selection;

    /* update the user interface */
    leds_off(0xff);
    if(current_selection != SELECTION_NONE){
        leds_on(1<<current_selection);
    }
}


/* button interrupt subroutine */
void button_isr(void) {
    piob->isr;

    int buttons = buttons_read();

    if(!buttons) return;

    /* use the leftmost pressed button */
    int selected = 0;
    while(buttons = buttons>>1){
        selected++;
    }

    select(selected);
}


/* abdac interrupt subroutine */
void abdac_isr(void) {
    int32_t out = 0;

    /* Play mod files */
    if(player->mod != NULL){
        out = MOD_Player_play(player);
        mod_ticks++;
    }

    /* Play sound effects */
    if(current_synth_sound != NULL){
        out = next_sample(current_synth_sound);
    }

    /* Force underrun. This is a HACK because setting
     * the ABDAC clock speed using div does not work. */
    volatile int slowalizer = 0;
    while(!dac->ISR.underrun){
        slowalizer += rand();
    }
    dac->ICR.underrun = 1;

    /* give the output to the hardware */
    dac->SDR.channel0 = out;
    dac->SDR.channel1 = out;
}
