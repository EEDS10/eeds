#include <allegro.h>
#include "State.h"

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
    textout_ex(screen, font, "Main menu, yo", 10, 10, makecol( 255, 0, 0), makecol( 0, 0, 0) );
}


static void state_update(){
    if (key[KEY_SPACE]){
        State_change(GameState);
    }
}

REGISTER_STATE(MainMenuState);
