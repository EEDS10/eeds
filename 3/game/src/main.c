#include <allegro.h>
#include "State.h"

extern State* MainMenuState;

int main(){

    int running = 1;

    allegro_init();
    install_keyboard();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);

    State_init();
    State_change(MainMenuState);

    while (running){
        clear_keybuf();
        State_update();

        acquire_screen();
        clear_bitmap(screen);
        State_render();
        release_screen();
        rest(50);
    }    

    State_deinit();
    return 0;

}   
END_OF_MAIN();
