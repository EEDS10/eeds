#include <allegro.h>
#include <stdio.h>
#include <dirent.h>
#include "State.h"
#include "SMSong.h"
#include "Font.h"

extern State* GameState;

Font* font_large;
Font* font_small;

SMSong** songs;
int n_songs;

bitmap_t* test;

static void state_init(){
    font_large = Font_load("res/fonts/font_large.bmp", 28, 36);
    font_small = Font_load("res/fonts/font_small.bmp", 20, 24);

    DIR *dp;
    struct dirent *ep;

    n_songs = 0;
    dp = opendir("res/Songs/");
    while(ep = readdir (dp)){
        n_songs++;
    }
    closedir(dp);
    songs = (SMSong**) malloc(sizeof(SMSong*) * n_songs);
    dp = opendir("res/Songs/");
    char filename[256];
    strcpy(filename, "res/Songs/");
    for(int i=0;i<n_songs;i++){
        ep = readdir(dp);
        if(ep->d_name[0] == '.') continue;
        strcpy(filename+10, ep->d_name);
        strcpy(filename+10 + strlen(ep->d_name), "/");
        strcpy(filename+10 + strlen(ep->d_name) + 1, "/");
        strcpy(filename+10 + strlen(ep->d_name) + 1, ep->d_name);
        strcpy(filename+10 + 2*strlen(ep->d_name) + 1, ".sm");
        printf("filename: %s\n", filename);
        songs[i] = SMSong_load(filename); 
    }
    closedir(dp);

    FILE* fp = fopen("res/test.bmp", "rw");
    test = eeds_load_bmp(fp);
    fclose(fp);
}


static void state_deinit(){

}


static void state_pause(){

}


static void state_resume(){

}


static void state_render(BITMAP* buffer){

    //eeds_render_bitmap(test, buffer->line, 0, 0);
    Font_render(font_small, buffer, "hello world", 5, 5);

}


static void state_update(){
    if (key[KEY_SPACE]){
        State_change(GameState);
    }
}

REGISTER_STATE(MainMenuState);
