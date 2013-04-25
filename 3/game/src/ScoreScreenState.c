#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif

#include "State.h"

extern State* MainMenuState;

static void state_init(){
}

static void state_deinit(){
}

static void state_pause(){
}

static void state_resume(){
}

static void state_render(bitmap_t* buffer){
}

static void state_update(){

    if(key[KEY_ESC]){
        State_change(MainMenuState);
    }

}

REGISTER_STATE(ScoreScreenState);
