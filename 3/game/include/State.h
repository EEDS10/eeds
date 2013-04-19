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

typedef struct State{
    void (*init)();
    void (*deinit)();
    void (*pause)();
    void (*resume)();
    void (*render)();
    void (*update)();
} State;

void State_change(State*state);
void State_render();
void State_update();
void State_init();
void State_deinit();

#endif
