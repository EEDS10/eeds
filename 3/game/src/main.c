#ifndef NO_ALLEGRO
    #include <allegro.h>
#else
    #include <linux/fb.h>
    #include <fcntl.h>
    #include <linux/soundcard.h>
    #include <sys/types.h>
    #include <sys/ioctl.h>
    #include "allegro_shim.h"
#endif
#include <stdio.h>
#include <linux/soundcard.h>
#include <stdlib.h>
#include "State.h"
#include "bmp_read.h"
#include "utils.h"

#define MILLISECONDS_PER_TICK 20

extern State* MainMenuState;
extern State* GameState;
int redraw_required = 1;

long t;
long old_t;
long dt;

FILE* audio;
int audio_progress;

bitmap_t* buffer;

#define SOUND_BUFFER_SIZE (1024*8)

int main(){

    int running = 1;

    int sound_tracker = 0;

    int sound = open("/dev/dsp", O_RDWR);
    int args, status;
    args = 16;
    status = ioctl(sound, SOUND_PCM_WRITE_BITS, &args);
    args = 2;
    status = ioctl(sound, SOUND_PCM_WRITE_CHANNELS, &args);
    args = 44100;
    status = ioctl(sound, SOUND_PCM_WRITE_RATE, &args);

    short sound_buffer[SOUND_BUFFER_SIZE];


    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);

    buffer = eeds_create_bitmap(320, 240);
    State_init(MainMenuState);
    State_init(GameState);
    State_change(MainMenuState);

    t = gettime();
    old_t = t;
    dt = 0;

    long bench_time;

    while(running){

        old_t = t;
        t = gettime();
        dt += t - old_t;

        while(dt > MILLISECONDS_PER_TICK){
            dt -= MILLISECONDS_PER_TICK;
            bench_time = gettime();
            clear_keybuf();
            State_update();
            //printf("[%lu:%lu] update()\n", dt, gettime() - bench_time);
        }

        if(redraw_required){
            bench_time = gettime();
            State_render(buffer);
            blit_to_screen(buffer, screen, 0, 0, 0, 0, 320, 240);
            redraw_required = 0;
            bench_time = gettime();
        }

        //printf("[%lu] render()\n", gettime() - bench_time);
        bench_time = gettime();
        if(audio != NULL){
            fread(sound_buffer, sizeof(short), SOUND_BUFFER_SIZE, audio);
            int written = write(sound, sound_buffer, sizeof(short)*SOUND_BUFFER_SIZE);
            audio_progress += written;
        }
        //printf("[%lu] render_sound()\n", gettime() - bench_time);
    }    

    State_deinit(MainMenuState);
    State_deinit(GameState);
    return 0;

}   
END_OF_MAIN();
