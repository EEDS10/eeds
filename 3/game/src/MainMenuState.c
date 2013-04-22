#include <allegro.h>
#include <stdio.h>
#include "State.h"
#include "SMSong.h"
#include "Font.h"

extern State* GameState;

FONT* font_large;
FONT* font_small;

static void state_init(){
    font_large = Font_load("res/fonts/font_large.bmp", 24, 24);
    font_small = Font_load("res/fonts/font_small.bmp", 16, 16);
}


static void state_deinit(){

}


static void state_pause(){

}


static void state_resume(){

}


static void state_render(){

}


static void state_update(){
    if (key[KEY_SPACE]){
        State_change(GameState);
    }
}

REGISTER_STATE(MainMenuState);
