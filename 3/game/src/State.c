#include <stdlib.h>
#ifndef NO_ALLEGRO
#include <allegro.h>
#endif
#include "State.h"

State* active_state;


void State_init(State*state){
    state->init();
}

void State_deinit(State*state){
    state->deinit();
}


void State_change(State*state){
    if(active_state != NULL){
        active_state->pause();
    }
    active_state = state;
    active_state->resume();
}

void State_render(BITMAP* buffer){
    active_state->render(buffer);
}


void State_update(){
    active_state->update();
}
