#include <allegro.h>
#include "State.h"
#include "Font.h"
#include "SMSong.h"

extern State* MainMenuState;

SMSong* song;
extern Font* font_small;
extern SMSong** songs;
extern int active_selection;
int elapsed_time_in_ms;

bitmap_t* note_sprite;

int beat;
int measure;
int beats_per_measure;

typedef struct Note{
    int x;
    int y;
} Note;

Note notes[256];
int n_notes = 0;

void add_note(int x, int y){
    notes[n_notes].x = x;
    notes[n_notes].y = y;
    n_notes++;
}

void remove_note(int i){
    n_notes--;
    notes[n_notes].x = notes[i].x;
    notes[n_notes].y = notes[i].y;
}

static void state_init(){
    FILE* fp = fopen("res/note_sprite.bmp", "rb");
    note_sprite = eeds_load_bmp(fp);
    fclose(fp);
}


static void state_deinit(){

}


static void state_pause(){

}


static void state_resume(){
    song = songs[active_selection];
    elapsed_time_in_ms = 0;
    beat = 0;
    measure = 0;
    beats_per_measure = 4;
}


static void state_render(BITMAP* buffer){

    for(int i=0;i<n_notes;i++){
        eeds_render_bitmap(note_sprite, buffer->line, notes[i].x, notes[i].y);
    }

    char elapsed_time_display_string[10];
    sprintf(elapsed_time_display_string, "%3.i.%1.is", elapsed_time_in_ms / 1000, (elapsed_time_in_ms % 1000) / 100);
    Font_render(font_small, buffer, elapsed_time_display_string, 5, 215);
}


static void state_update(){
    if(key[KEY_ESC]){
        State_change(MainMenuState);
    }


    for(int i=0;i<n_notes;i++){
        notes[i].y--;
    }


    if(elapsed_time_in_ms % 1000){
        add_note(10, 240);
    }

    elapsed_time_in_ms += 20;
}

REGISTER_STATE(GameState);
