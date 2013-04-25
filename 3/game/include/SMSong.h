#ifndef SMSONG_H
#define SMSONG_H


typedef struct Measure{
    char rows[12][4];
    int n_rows;
} Measure;

typedef struct SMSong{
    char* title;
    char* subtitle;
    char* artist;
    char* titletranslit;
    char* subtitletranslit;
    char* artisttranslit;
    char* credit;
    char* banner;
    char* background;
    char* lyricspath;
    char* cdtitle;
    char* music;
    int offset_in_ms;
    int samplestart_in_ms;
    int samplelength_in_ms;
    int selectable;
    int* BPMs;
    int* stops;
    int* BGchanges;
    void* notes;    
    Measure** measures;
    char* basename;
} SMSong;


SMSong* SMSong_load(char* filename, char* basename);

#endif
