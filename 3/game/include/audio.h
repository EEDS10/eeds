#ifndef AUDIO_H
#define AUDIO_H

#include <linux/soundcard.h>
#include <stdio.h>

#define SOUND_BUFFER_SIZE (1024*8)

extern FILE* audio;
extern int audio_progress;

void audio_init();

void audio_play(char* filename);
void audio_done();
void audio_drive();

#endif
