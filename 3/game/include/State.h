#ifndef STATE_H
#define STATE_H

void State_init(State*state);
void State_deinit(State*state);

void State_pause(State*state);
void State_resume(State*state);

void State_render(State*state);
void State_update(State*state);

#endif
