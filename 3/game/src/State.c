#include <stdlib.h>
#include "State.h"

State* active_state;


void State_init(State*state){
}
void State_deinit(){
}


void State_change(State*state){
    if(active_state != NULL){
        active_state->pause();
    }
    active_state = state;
    active_state->resume();
}

void State_render(){
    active_state->render();
}


void State_update(){
    active_state->update();
}
