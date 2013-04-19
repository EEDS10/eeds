#include <allegro.h>
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


static void state_render(){
    textout_ex(screen, font, "This is the main game", 100, 10, makecol( 0, 255, 0), makecol( 0, 0, 0) );
}


static void state_update(){
    if(key[KEY_ESC]){
        State_change(MainMenuState);
    }
}

REGISTER_STATE(GameState);
