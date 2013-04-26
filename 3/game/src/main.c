#ifndef NO_ALLEGRO
    #include <allegro.h>
#else
    #include <linux/fb.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/ioctl.h>
    #include "allegro_shim.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include "State.h"
#include "bmp_read.h"
#include "utils.h"
#include "audio.h"

#define MILLISECONDS_PER_TICK 20

extern State* IntroState;
extern State* MainMenuState;
extern State* GameState;
int redraw_required = 1;

long t;
long old_t;
long dt;


bitmap_t* buffer;


int main(){
    int running = 1;

    audio_init();
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);

    buffer = eeds_create_bitmap(320, 240);

    State_init(IntroState);
    State_init(MainMenuState);
    State_init(GameState);
    State_change(IntroState);

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
        }

        if(redraw_required){
            bench_time = gettime();
            State_render(buffer);
            blit_to_screen(buffer, screen, 0, 0, 0, 0, 320, 240);
            redraw_required = 0;
            bench_time = gettime();
        }

        bench_time = gettime();

        audio_drive();
    }

    State_deinit(MainMenuState);
    State_deinit(GameState);

    return 0;
}
END_OF_MAIN();
