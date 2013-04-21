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
    SMSong* song = SMSong_load("res/Songs/Plaguemix Series/I Turn to You/iturntoyou.sm");
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
