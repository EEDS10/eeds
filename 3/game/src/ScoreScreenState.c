#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif
#include "State.h"
#include "utils.h"
#include "Font.h"
#include "leds.h"
#include "SMSong.h"

extern State* MainMenuState;

extern SMSong* song;

extern int score_perfect;
extern int score_great;
extern int score_OK;
extern int score_combo;
extern int score_greatest_combo;
extern int total_notes;

bitmap_t* score_bg;

extern Font* font_small;
extern Font* font_large;

extern int redraw_required;

static void state_init(){
    eeds_set_leds(1);
}

static void state_deinit(){
}

static void state_pause(){
    eeds_free_bitmap(score_bg);
}

static void state_resume(){
    printf("entered SSS\n");

    eeds_set_leds(1);
    redraw_required = 1;

    FILE*fp = fopen("res/score_bg.bmp", "rb");
    if(fp == NULL){
        printf("DAYMn that score_bg was not even loaded!\n");
    }
    score_bg = eeds_load_bmp(fp);
    fclose(fp);
    printf("resume done\n");
}

static void state_render(bitmap_t* buffer){

    printf("before blit \n");
    eeds_render_bitmap(score_bg, buffer, 0, 0);
    printf("after blit \n");

    char message[30];
    sprintf(message, "PERFECT! %4.i", score_perfect);
    Font_render(font_small, buffer, message, 5, 15, 9);

    sprintf(message, "Great!   %4.i", score_great);
    Font_render(font_small, buffer, message, 5, 57, 9);

    sprintf(message, "O.K.     %4.i", score_OK);
    Font_render(font_small, buffer, message, 5, 97, 9);

    int miss = total_notes - score_perfect - score_great - score_OK;
    sprintf(message, "Miss     %4.i", miss);
    Font_render(font_small, buffer, message, 5, 137, 9);

    int score = score_perfect * 8 +
                score_great * 5 +
                score_OK * 3 +
                miss * -2;
    Font_render(font_small, buffer, "Score:", 180, 107, 9);
    sprintf(message, "%8.i", score);
    Font_render(font_small, buffer, message, 180, 137, 9);

    sprintf(message, "Best combo: %i", score_greatest_combo);
    Font_render(font_large, buffer, message, 5, 175, 11);

    Font_render(font_large, buffer, "GREAT, YOU ROCK!", 5, 205, 11);
}

static void state_update(){


    if(key[KEY_ESC]){
        State_change(MainMenuState);
    }
}

REGISTER_STATE(ScoreScreenState);
