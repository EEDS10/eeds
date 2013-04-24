#ifndef STATE_H
#define STATE_H

#define REGISTER_STATE(name) static State _state = { \
    .init = state_init, \
    .deinit = state_deinit, \
    .pause = state_pause, \
    .resume = state_resume, \
    .render = state_render, \
    .update = state_update, \
}; State* name = &_state;

#ifdef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif

typedef struct State{
    void (*init)();
    void (*deinit)();
    void (*pause)();
    void (*resume)();
    void (*render)(BITMAP*);
    void (*update)();
} State;

void State_change(State*state);
void State_render(BITMAP* buffer);
void State_update();
void State_init(State* state);
void State_deinit(State* state);

#endif
