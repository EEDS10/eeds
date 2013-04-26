#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif
#include <stdlib.h>
#include "leds.h"
#include "State.h"
#include "Font.h"
#include "SMSong.h"
#include "utils.h"

extern State* MainMenuState;
extern State* ScoreScreenState;

extern FILE* audio;
extern int audio_progress;

SMSong* song;
extern Font* font_small;
extern Font* font_large;
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
int need_to_draw_song_bg;

int score_OK;
int score_great;
int score_perfect;
int score_combo;
int score_greatest_combo;
int total_notes;

char* accuracy_feedback;
int feedback_counter;

char* accuracy_OK =      "  O.K.";
char* accuracy_great =   " Great!";
char* accuracy_perfect = "PERFECT!";

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
    printf("init done!\n");
}


static void state_deinit(){

}


static void state_pause(){
    eeds_free_bitmap(song_bg);
    eeds_free_bitmap(game_bg);
    fclose(audio);
    audio = NULL;
}

static int isTransparent(colour_t colour);

static void merge_bgs() {
    /*  bitmap_t has width, height and colour_t **bitmap
        need to start the loop at x =
        (songbg->width - game_bg->width)/2
        x = [(songbg->width - game_bg-width)/2, (songbg->width - game_bg->width)/2 + game_bg->width]
        y = [0, songbg->height]
     */
    int x = (song_bg->width - game_bg->width)/2;
    /* int endx = startx + game_bg->width; */
    /*
        (value1 * a + value2*(255-a))/255
        printf("merge yo\n");
        */
    int overlayAlpha = 180;
    for (int i = 0; i < game_bg->width; i++, x++) {
        for (int j = 0; j < game_bg->height; j++) {
            /* uh, ok, how do I create fake transparency? */
            /* whatever I'll just overwrite game_bg */
            if (isTransparent(game_bg->bitmap[j][i])) {
                game_bg->bitmap[j][i] = song_bg->bitmap[j][x];
            } else {
                game_bg->bitmap[j][i].red =
                    (game_bg->bitmap[j][i].red * overlayAlpha +
                     song_bg->bitmap[j][x].red*(255 - overlayAlpha)
                    )/255;
                game_bg->bitmap[j][i].blue =
                    (game_bg->bitmap[j][i].blue*overlayAlpha
                     + song_bg->bitmap[j][x].blue*(255 - overlayAlpha)
                    )/255;
                game_bg->bitmap[j][i].green =
                    (game_bg->bitmap[j][i].green*overlayAlpha
                     + song_bg->bitmap[j][x].green*(255 - overlayAlpha)
                    )/255;
            }
        }
    }
}


static void state_resume(){
    total_notes = 0;
    eeds_set_leds(0xAA);
    score_OK = 0;
    score_perfect = 0;
    score_great = 0;
    score_combo = 0;
    score_greatest_combo = 0;
    song = songs[active_selection];
    elapsed_time_in_ms = -song->offset_in_ms - 43;
    beat = 0;
    measure = 0;
    beats_per_measure = 4;
    ms_since_last_beat = 0;
    current_bpm_index = 1;
    current_bpm = song->BPMs[current_bpm_index];
    need_to_draw_song_bg = 1;
    ms_per_measure = 4*60000000 / current_bpm;

    char path_to_song[256];
    sprintf(path_to_song, "res/Songs/%s/%s.raw", song->basename, song->basename);
    audio = fopen(path_to_song, "rb");

    char path_to_bg[256];
    sprintf(path_to_bg, "res/Songs/%s/%s.bmp", song->basename, song->basename);
    /* load song background */
    FILE* fp = fopen(path_to_bg, "rb");
    song_bg = eeds_load_bmp(fp);
    fclose(fp);
    /* load background overlay */
    fp = fopen("res/game_bg.bmp", "rb");
    game_bg = eeds_load_bmp(fp);
    fclose(fp);
    /* merge song and background overlay */
    merge_bgs();
}

/* Returns true if given colour is transparent
    bmp transparency == r=255, b=255, g=0 */
static int isTransparent(colour_t colour) {
    if (colour.red == 255 && colour.blue == 255 && colour.green == 0) {
        return 1;
    }
    return 0;
}

static void state_render(bitmap_t* buffer){
    if(need_to_draw_song_bg){
        eeds_render_bitmap(song_bg, buffer, 0, 0);
        need_to_draw_song_bg = 0;

    }
    eeds_render_bitmap(game_bg, buffer, 64, 0);

    for(int i=0;i<n_notes;i++){
        eeds_render_bitmap(note_sprite, buffer, 72 + notes[i].column * 50, notes[i].y);
    }

    if(key[KEY_A]){
        eeds_render_bitmap(note_sprite, buffer, 72 + 0 * 50, 29);
    }

    if(key[KEY_S]){
        eeds_render_bitmap(note_sprite, buffer, 72 + 1 * 50, 29);
    }

    if(key[KEY_D]){
        eeds_render_bitmap(note_sprite, buffer, 72 + 2 * 50, 29);
    }

    if(key[KEY_F]){
        eeds_render_bitmap(note_sprite, buffer, 72 + 3 * 50, 29);
    }

    if(accuracy_feedback != NULL && feedback_counter > 0){
        Font_render(font_small, buffer, accuracy_feedback, 120, 4, 9);
    }

    if(score_combo > 3){
        char combo_score[30];
        sprintf(combo_score, "%i combo!", score_combo);
        Font_render(font_large, buffer, combo_score, 80, 100, 9);
    }
}

void hit_notes(int column){
    int note_hit_height = 20;
    for(int i=0;i<n_notes;i++){
        if(notes[i].column == column){
            int difference = abs(notes[i].y - 40 + note_sprite->height);
            if(difference < 4){
                accuracy_feedback = accuracy_perfect;
                score_perfect++;
                score_combo++;
                feedback_counter = 20;
                remove_note(i--);
            }else if(difference < 10){
                accuracy_feedback = accuracy_great;
                score_combo++;
                score_great++;
                feedback_counter = 20;
                remove_note(i--);
            }else if(difference < 20){
                accuracy_feedback = accuracy_OK;
                score_OK++;
                score_combo++;
                feedback_counter = 20;
                remove_note(i--);
            }
        }
    }
}

int audio_time = 0;
int old_audio_time = 0;
int audio_dt = 0;


static void state_update(){

    old_audio_time = audio_time;
    audio_time = audio_progress;
    audio_dt += audio_time - old_audio_time;
    while(audio_dt >= 441*2){
        audio_dt -= 441*2;
        ms_since_last_beat += 5;

        for(int i=0;i<n_notes;i++){
            notes[i].y--;
            if(notes[i].done || notes[i].y < -30){
                score_greatest_combo = MAX(score_greatest_combo, score_combo);
                score_combo = 0;
                remove_note(i);
                i--;
            }
        }
    }

    redraw_required = 1;

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

    if(feedback_counter > 0){
        feedback_counter--;
    }


    int ms_per_beat = ms_per_measure / beats_per_measure;
    while(ms_since_last_beat > ms_per_beat){
        ms_since_last_beat -= ms_per_beat;
        beat++;
        if(beat > beats_per_measure){
            beat -= beats_per_measure;
            measure++;
            if(song->measures[measure] == NULL){
                State_change(ScoreScreenState);
                return;
            }
            beats_per_measure = song->measures[measure]->n_rows;
            if(measure == song->BPMs[current_bpm_index + 1]){
                current_bpm_index += 2;
                current_bpm = song->BPMs[current_bpm_index];
                ms_per_measure = 4*60000000 / current_bpm;
            }
        }
        for(int i=0;i<4;i++){
            if(song->measures[measure]->rows[beat][i] == '1'){
                add_note(i, 244);
                total_notes++;
            }
        }
    }

}

REGISTER_STATE(GameState);
