#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif
#include "State.h"
#include "utils.h"
#include "Font.h"

extern State* MainMenuState;

extern int score_perfect;
extern int score_great;
extern int score_OK;
extern int score_combo;
extern int score_greatest_combo;

extern Font* font_small;
extern Font* font_large;

extern int redraw_required;

static void state_init(){
}

static void state_deinit(){
}

static void state_pause(){
}

static void state_resume(){
    redraw_required = 1;
}

static void state_render(bitmap_t* buffer){

    clear_to_color(buffer,makecol(255,255,255));

    char message[30];
    sprintf(message, "PERFECT!: %i", score_perfect);
    Font_render(font_small, buffer, message, 5, 15, 9);

    sprintf(message, "Great!: %i", score_great);
    Font_render(font_small, buffer, message, 5, 45, 9);

    sprintf(message, "O.K.: %i", score_OK);
    Font_render(font_small, buffer, message, 5, 75, 9);

    sprintf(message, "Best combo: %i", score_greatest_combo);
    Font_render(font_large, buffer, message, 5, 135, 11);
    Font_render(font_large, buffer, "GREAT, YOU ROCK!", 5, 185, 11);
}

static void state_update(){


    if(key[KEY_ESC]){
        State_change(MainMenuState);
    }
}

REGISTER_STATE(ScoreScreenState);
