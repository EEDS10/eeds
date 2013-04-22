#include <allegro.h>
#include <stdio.h>
#include "State.h"
#include "SMSong.h"
#include "Font.h"

extern State* GameState;

Font* font_large;
Font* font_small;

bitmap_t* test;

static void state_init(){
    font_large = Font_load("res/fonts/font_large.bmp", 24, 24);
    font_small = Font_load("res/fonts/font_small.bmp", 16, 16);

    FILE* fp = fopen("res/test.bmp", "rw");
    test = eeds_load_bmp(fp);
    fclose(fp);
}


static void state_deinit(){

}


static void state_pause(){

}


static void state_resume(){

}


static void state_render(BITMAP* buffer){

    //Font_render(font_large, screen, "hello world", 5, 5);
    //render_bitmap(font_large->bmp, buffer->line, 0, 0);
    eeds_render_bitmap(test, buffer->line, 0, 0);

}


static void state_update(){
    if (key[KEY_SPACE]){
        State_change(GameState);
    }
}

REGISTER_STATE(MainMenuState);
