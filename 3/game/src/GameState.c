#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif
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

int current_bpm;
int current_bpm_index;
int beat;
int measure;
int beats_per_measure;
int ms_per_measure;
int ms_since_last_beat;

typedef struct Note{
    int x;
    int y;
    int done;
} Note;

Note notes[256];
int n_notes = 0;

void add_note(int x, int y){
    notes[n_notes].x = x;
    notes[n_notes].y = y;
    notes[n_notes].done = 0;
    n_notes++;
}

void remove_note(int i){
    n_notes--;
    notes[i].x = notes[n_notes].x;
    notes[i].y = notes[n_notes].y;
    notes[i].done = notes[n_notes].done;
}

static void state_init(){
    printf("gamestate init\n");
    FILE* fp = fopen("res/note_sprite.bmp", "rb");
    note_sprite = eeds_load_bmp(fp);
    fclose(fp);
    printf("init done!\n");
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
    ms_per_measure = 800;
    ms_since_last_beat = 0;
    current_bpm_index = 1;
    current_bpm = song->BPMs[current_bpm_index];
}


static void state_render(bitmap_t* buffer){

    for(int i=0;i<n_notes;i++){
        eeds_render_bitmap(note_sprite, buffer, notes[i].x, notes[i].y);
    }

    char elapsed_time_display_string[10];
    sprintf(elapsed_time_display_string, "%3.i.%1.is", elapsed_time_in_ms / 1000, (elapsed_time_in_ms % 1000) / 100);
    Font_render(font_small, buffer, elapsed_time_display_string, 5, 215, 9);
}


static void state_update(){
    if(key[KEY_ESC]){
        State_change(MainMenuState);
    }


    for(int i=0;i<n_notes;i++){
        notes[i].y-= 2;

        if(notes[i].done || notes[i].y < -30){
            remove_note(i);
            i--;
        }
    }


    int ms_per_beat = ms_per_measure / beats_per_measure;
    while(ms_since_last_beat > ms_per_beat){
        ms_since_last_beat -= ms_per_beat;
        beat++;
        if(beat > beats_per_measure){
            beat -= beats_per_measure;
            measure++;
            beats_per_measure = song->measures[measure]->n_rows;
            if(measure == song->BPMs[current_bpm_index + 1]){
                current_bpm_index += 2;
                current_bpm = song->BPMs[current_bpm_index]; 

                ms_per_measure = (current_bpm * 60 * 1000) / 4;
            }
        }
        for(int i=0;i<4;i++){
            if(song->measures[measure]->rows[beat][i] == '1'){
                add_note(10 + 30*i, 240);
            }
        }
    }

    ms_since_last_beat += 20;
    elapsed_time_in_ms += 20;
}

REGISTER_STATE(GameState);
