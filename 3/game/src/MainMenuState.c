#include <allegro.h>
#include <stdio.h>
#include "State.h"
#include "SMSong.h"

extern State* GameState;

static void state_init(){
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
