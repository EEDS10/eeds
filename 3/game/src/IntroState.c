#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "State.h"
#include "SMSong.h"
#include "Font.h"

extern State* MainMenuState;

bitmap_t* intro_bg;
bitmap_t* fire_palette;

char **fire_array;

int draw_bg;

static void state_init(){
    FILE* fp = fopen("res/title.bmp", "rw");
    intro_bg = eeds_load_bmp(fp);
    fclose(fp);

    fp = fopen("res/fire.bmp", "rw");
    fire_palette = eeds_load_bmp(fp);
    fclose(fp);

    fire_array = malloc(sizeof(*fire_array) * 130);

    for (int i = 0; i < 130; i++) {
        /* Initialise an array of zeroes. */
        fire_array[i] = calloc(sizeof(**fire_array), 320);
    }

    draw_bg = 1;

    printf("init done!\n");
}


static void state_deinit(){
    eeds_free_bitmap(intro_bg);

    for (int i = 0; i < 130; i++) {
        free(fire_array[i]);
    }
    free(fire_array);
}


static void state_pause(){
}


static void state_resume(){
}


static void state_render(bitmap_t* buffer){
    if (draw_bg) {
        eeds_render_bitmap(intro_bg, buffer, 0, 0);
        draw_bg = 0;
    }

    /* Ugly, but speedier. */
    for (int y = 0; y < 329; y++) {
        fire_array[0][y] = rand() % 256;
        buffer->bitmap[239][y] = fire_palette->bitmap[0][fire_array[0][y]];
    }

    for (int y = 0; y < 329; y++) {
        fire_array[1][y] = rand() % 256;
        buffer->bitmap[238][y] = fire_palette->bitmap[0][fire_array[1][y]];
    }

    for (int x = 1; x < 128; x++) {
        for (int y = 1; y < 329; y++) {
            //fire_array[x+1][y] = (fire_array[x-1][y] + fire_array[x+1][y] + fire_array[x][y-1] + fire_array[x][y+1]) / 3;
            fire_array[x+1][y] = (fire_array[x-1][y] + fire_array[x+1][y] + fire_array[x][y-1] + fire_array[x][y+1] + fire_array[x-1][y-1] + fire_array[x-1][y+1] + fire_array[x+1][y-1] + fire_array[x+1][y+1]) / 7;
            buffer->bitmap[239-x][y] = fire_palette->bitmap[0][fire_array[x][y]];
        }
    }
}


static void state_update(){
    if (key[KEY_SPACE]){
        State_change(MainMenuState);
    }

    redraw_required = 1;
}

REGISTER_STATE(IntroState);
