#include <allegro.h>
#include <stdio.h>
#include "State.h"
#include "bmp_read.h"
#include "utils.h"

extern State* MainMenuState;
extern State* GameState;

BITMAP* buffer;

int main(){

    int running = 1;

    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);

    buffer = create_bitmap(320, 240);
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
            dt -= MILLISECONDS_PER_TICK;
            clear_keybuf();
            State_update();
            redraw_required = 1;
        }

        if(redraw_required){
            clear_to_color(buffer, makecol(255,255,255));
            State_render(buffer);
            blit(buffer, screen, 0, 0, 0, 0, 320, 240);
            redraw_required = 0;
        }

        rest(50);
    }    

    State_deinit(MainMenuState);
    State_deinit(GameState);
    return 0;

}   
END_OF_MAIN();
