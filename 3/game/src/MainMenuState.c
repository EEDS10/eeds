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
#include "leds.h"

extern State* GameState;

Font* font_large;
Font* font_small;

SMSong** songs;
int n_songs;

int active_selection = 0;

int key_cooldown = 0;

bitmap_t* menu_bg;

static void state_init(){
    font_large = Font_load("res/fonts/font_large.bmp", 28, 36);
    font_small = Font_load("res/fonts/font_small.bmp", 20, 24);

    n_songs = 6;
    songs = malloc(sizeof(SMSong*)*n_songs);

    songs[0] = SMSong_load("res/Songs/32a5/32a5.sm", "32a5");
    songs[1] = SMSong_load("res/Songs/iturntoyou/iturntoyou.sm", "iturntoyou");
    songs[2] = SMSong_load("res/Songs/jumparound/jumparound.sm", "jumparound");
    songs[3] = SMSong_load("res/Songs/lost/lost.sm", "lost");
    songs[4] = SMSong_load("res/Songs/supertrouper/supertrouper.sm", "supertrouper");
    songs[5] = SMSong_load("res/Songs/whothehell/whothehell.sm", "whothehell");

    for(int i=0;i<n_songs;i++){
        if(songs[i] == NULL){
            printf("[%i] NULL!\n", i);
        }
        printf("[%i] TITLE: %s\n", i, songs[i]->title);
    }

    FILE* fp = fopen("res/menu.bmp", "rw");
    menu_bg = eeds_load_bmp(fp);
    fclose(fp);
}


static void state_deinit(){
}


static void state_pause(){
}


static void state_resume(){
    printf("main menu resume\n");
    redraw_required = 1;
    eeds_set_leds(0xE);
}


static void state_render(bitmap_t* buffer){
    eeds_render_bitmap(menu_bg, buffer, 0, 0);

    Font_render(font_small, buffer, songs[(active_selection + n_songs - 2) % n_songs]->title, 2, 15, 9);
    Font_render(font_small, buffer, songs[(active_selection + n_songs - 1) % n_songs]->title, 2, 57, 9);
    Font_render(font_large, buffer, songs[(active_selection + n_songs    ) % n_songs]->title, 2, 100, 11);
    Font_render(font_small, buffer, songs[(active_selection + n_songs + 1) % n_songs]->title, 2, 157, 9);
    Font_render(font_small, buffer, songs[(active_selection + n_songs + 2) % n_songs]->title, 2, 197, 9);

    Font_render(font_small, buffer, "by:", 190, 37, 9);
    Font_render(font_small, buffer, songs[(active_selection + n_songs) % n_songs]->artist, 190, 57, 9);
    char bpm_string[30];
    sprintf(bpm_string, "BPM: %i", songs[(active_selection + n_songs) % n_songs]->BPMs[1]/1000);
    Font_render(font_small, buffer, bpm_string, 190, 157, 9);
}


static void state_update(){
    if (key[KEY_SPACE]){
        State_change(GameState);
    }

    if(key_cooldown == 0){
        if (key[KEY_UP]){
            active_selection--;
            key_cooldown = 7;
            redraw_required = 1;
        }
        if (key[KEY_DOWN]){
            active_selection++;
            key_cooldown = 7;
            redraw_required = 1;
        }
    }else{
        key_cooldown--;
    }

    active_selection = (active_selection + n_songs) % n_songs;
}

REGISTER_STATE(MainMenuState);
