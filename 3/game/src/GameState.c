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
bitmap_t* song_bg;
bitmap_t* game_bg;

int current_bpm;
int current_bpm_index;
int beat;
int measure;
int beats_per_measure;
int ms_per_measure;
int ms_since_last_beat;

typedef struct Note{
    int column;
    int y;
    int done;
} Note;

Note notes[256];
int n_notes = 0;

void add_note(int column, int y){
    notes[n_notes].column = column;
    notes[n_notes].y = y;
    notes[n_notes].done = 0;
    n_notes++;
}

void remove_note(int i){
    n_notes--;
    notes[i].column = notes[n_notes].column;
    notes[i].y = notes[n_notes].y;
    notes[i].done = notes[n_notes].done;
}

static void state_init(){
    printf("gamestate init\n");
    FILE* fp = fopen("res/note_sprite.bmp", "rb");
    note_sprite = eeds_load_bmp(fp);
    fclose(fp);
    fp = fopen("res/game_bg.bmp", "rb");
    game_bg = eeds_load_bmp(fp);
    fclose(fp);
    printf("init done!\n");
}


static void state_deinit(){

}


static void state_pause(){
    eeds_free_bitmap(song_bg);
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

    char path_to_bg[256];
    sprintf(path_to_bg, "res/Songs/%s/%s.bmp", song->basename, song->basename);
    FILE* fp = fopen(path_to_bg, "rb");
    song_bg = eeds_load_bmp(fp);
    fclose(fp);
}


static void state_render(bitmap_t* buffer){

    eeds_render_bitmap(song_bg, buffer, 0, 0);
    eeds_render_bitmap(game_bg, buffer, 0, 0);

    for(int i=0;i<n_notes;i++){
        eeds_render_bitmap(note_sprite, buffer, 80 + notes[i].column * 40, notes[i].y);
    }

    char elapsed_time_display_string[10];
    sprintf(elapsed_time_display_string, "%3.i.%1.is", elapsed_time_in_ms / 1000, (elapsed_time_in_ms % 1000) / 100);
    Font_render(font_small, buffer, elapsed_time_display_string, 5, 215, 9);
}

void hit_notes(int column){
    int note_hit_height = 20;
    for(int i=0;i<n_notes;i++){
        if(notes[i].column == column){
            int difference = abs(notes[i].y - 40);
            if(difference == 0){
                printf("PERFECT!\n");
                remove_note(i--);
            }else if(difference < 4){
                printf("Great!\n");
                remove_note(i--);
            }else if(difference < 10){
                printf("OK\n");
                remove_note(i--);
            }else if(difference < 20){
                printf("You suck\n");
            }
        }
    }
}


static void state_update(){
    if(key[KEY_ESC]){
        State_change(MainMenuState);
    }

    if(key[KEY_A]){
        hit_notes(0);     
    }

    if(key[KEY_S]){
        hit_notes(1);     
    }

    if(key[KEY_D]){
        hit_notes(2);     
    }

    if(key[KEY_F]){
        hit_notes(3);     
    }


    for(int i=0;i<n_notes;i++){
        notes[i].y -= 3;

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
            printf("UNTS\n");
            beats_per_measure = song->measures[measure]->n_rows;
            if(measure == song->BPMs[current_bpm_index + 1]){
                current_bpm_index += 2;
                current_bpm = song->BPMs[current_bpm_index]; 
                ms_per_measure = (current_bpm * 60 * 1000) / 4;
            }
        }
        for(int i=0;i<4;i++){
            if(song->measures[measure]->rows[beat][i] == '1'){
                add_note(i, 240);
            }
        }
    }

    ms_since_last_beat += 20;
    elapsed_time_in_ms += 20;
}

REGISTER_STATE(GameState);
