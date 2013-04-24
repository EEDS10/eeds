#ifndef NO_ALLEGRO
    #include <allegro.h>
#else
    #include <linux/fb.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/mman.h>
    #include "allegro_shim.h"
#endif
#include <stdio.h>
#include <linux/soundcard.h>
#include <stdlib.h>
#include "State.h"
#include "bmp_read.h"
#include "utils.h"

extern State* MainMenuState;
extern State* GameState;

bitmap_t* buffer;

int main(){

    int running = 1;

#ifdef NO_ALLEGRO

    int screen_size = 320*240*3;
    int framebuffer;
    if(!(framebuffer = open("/dev/fb0", O_RDWR))){
        printf("Failed to open framebuffer.\n"); 
    }
    unsigned char* screen = mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, framebuffer, 0);
    if((int)screen == -1){
        printf("Failed to mmap the framebuffer.\n");
    }
#endif

    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);

    buffer = eeds_create_bitmap(320, 240);
    State_init(MainMenuState);
    State_init(GameState);
    State_change(MainMenuState);

    long t = gettime();
    long old_t = t;
    long dt = 0;

    int redraw_required = 0;

    while(running){

        old_t = t;
        t = gettime();
        dt += t - old_t;

        while(dt > MILLISECONDS_PER_TICK){
            printf("[%lu:%lu] update\n", t, dt);
            dt -= MILLISECONDS_PER_TICK;
            clear_keybuf();
            State_update();
            redraw_required = 1;
        }

        if(redraw_required){
            printf("[%lu:%lu] render\n", t, dt);
            eeds_clear_to_color(buffer, 255, 255, 255);
            State_render(buffer);
            blit_to_screen(buffer, screen, 0, 0, 0, 0, 320, 240);
            redraw_required = 0;
        }
    }    

    State_deinit(MainMenuState);
    State_deinit(GameState);
    return 0;

}   
END_OF_MAIN();
