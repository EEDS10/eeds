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

extern State* GameState;

Font* font_large;
Font* font_small;

SMSong** songs;
int n_songs;

int active_selection = 0;

int key_cooldown = 0;

bitmap_t* menu_bg;

static void state_init(){
    printf("main_menu_init()\n");
    font_large = Font_load("res/fonts/font_large.bmp", 28, 36);
    font_small = Font_load("res/fonts/font_small.bmp", 20, 24);
    printf("done loading fonts...\n");

    n_songs = 3;
    songs = malloc(sizeof(SMSong*)*n_songs);

    songs[0] = SMSong_load("res/Songs/32a5/32a5.sm");
    songs[1] = SMSong_load("res/Songs/iturntoyou/iturntoyou.sm");
    songs[2] = SMSong_load("res/Songs/lost/lost.sm");
    for(int i=0;i<n_songs;i++){
        if(songs[i] == NULL){
            printf("[%i] NULL!\n", i);
        }
        printf("[%i] TITLE: %s\n", i, songs[i]->title);
    }

    FILE* fp = fopen("res/menu.bmp", "rw");
    menu_bg = eeds_load_bmp(fp);
    fclose(fp);
    printf("init done!\n");
}


static void state_deinit(){

}


static void state_pause(){

}


static void state_resume(){

}


static void state_render(bitmap_t* buffer){
    printf("mmstate render\n");

    eeds_render_bitmap(menu_bg, buffer, 0, 0);
    Font_render(font_small, buffer, songs[(active_selection + n_songs - 2) % n_songs]->title, 2, 15);
    Font_render(font_small, buffer, songs[(active_selection + n_songs - 1) % n_songs]->title, 2, 57);
    Font_render(font_large, buffer, songs[(active_selection + n_songs    ) % n_songs]->title, 2, 100);
    Font_render(font_small, buffer, songs[(active_selection + n_songs + 1) % n_songs]->title, 2, 157);
    Font_render(font_small, buffer, songs[(active_selection + n_songs + 2) % n_songs]->title, 2, 197);
}


static void state_update(){
    if (key[KEY_SPACE]){
        State_change(GameState);
    }
    if(key_cooldown == 0){
        if (key[KEY_UP]){
            active_selection--;
            key_cooldown = 5;
        }
        if (key[KEY_DOWN]){
            active_selection++;
            key_cooldown = 5;
        }
    }else{
        key_cooldown--;
    }

    active_selection = (active_selection + n_songs + 1) % n_songs;
}

REGISTER_STATE(MainMenuState);
