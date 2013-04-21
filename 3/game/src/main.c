#include <allegro.h>
#include <stdio.h>
#include "State.h"
#include "utils.h"

extern State* MainMenuState;

int main(){

    int running = 1;

    allegro_init();
    install_keyboard();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);

    State_init();
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
            dt -= MILLISECONDS_PER_TICK;
            clear_keybuf();
            State_update();
            redraw_required = 1;
        }

        if(redraw_required){
            acquire_screen();
            clear_bitmap(screen);
            State_render();
            release_screen();
            redraw_required = 0;
        }

        rest(50);
    }    

    State_deinit();
    return 0;

}   
END_OF_MAIN();
