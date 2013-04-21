#ifndef SMSONG_H
#define SMSONG_H

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
    char* BPMs;
    char* stops;
    char* BGchanges;
    void* notes;    
} SMSong;


SMSong* SMSong_load(char* filename);

#endif
