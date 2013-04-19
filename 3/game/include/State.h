#ifndef STATE_H
#define STATE_H

#define CREATE_STATE(name) State MainMenuState = { \
    .init = init, \
    .deinit = deinit, \
    .pause = pause, \
    .resume = resume, \
    .render = render, \
    .update = update, \
};

typedef struct State{
    void* init;
    void* deinit;
    void* pause;
    void* resume;
    void* render;
    void* update;
} State;

void State_init(State*state);
void State_deinit(State*state);

void State_pause(State*state);
void State_resume(State*state);

void State_render(State*state);
void State_update(State*state);

#endif
