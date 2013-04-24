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

#include "bmp_read.h"

#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif

typedef struct State{
    void (*init)();
    void (*deinit)();
    void (*pause)();
    void (*resume)();
    void (*render)(bitmap_t*);
    void (*update)();
} State;

void State_change(State*state);
void State_render(bitmap_t* buffer);
void State_update();
void State_init(State* state);
void State_deinit(State* state);

#endif
