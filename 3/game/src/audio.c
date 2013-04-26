#include <linux/soundcard.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "audio.h"

FILE* audio;
int audio_progress;
short sound_buffer[SOUND_BUFFER_SIZE];
int sound;

void audio_done(){
    if(audio != NULL){
        fclose(audio);
        audio = NULL;
    }
}

void audio_play(char* filename){
    audio_done();
    audio = fopen(filename, "r"); 
}

void audio_drive(){
    if(audio != NULL){
        fread(sound_buffer, sizeof(short), SOUND_BUFFER_SIZE, audio);
        int written = write(sound, sound_buffer, sizeof(short)*SOUND_BUFFER_SIZE);
        audio_progress += written;
    }
}

void audio_init(){
    int args, status;
    sound = open("/dev/dsp", O_RDWR);

    args = 16;
    status = ioctl(sound, SOUND_PCM_WRITE_BITS, &args);
    args = 2;
    status = ioctl(sound, SOUND_PCM_WRITE_CHANNELS, &args);
    args = 44100;
    status = ioctl(sound, SOUND_PCM_WRITE_RATE, &args);
}
