#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "SMSong.h"


#define BEGINPARSE() do{ int n; if(0){
#define ENDPARSE() }}while(0);
#define PARSE(key, target) \
    }else if(strbeginswith(line, key)){ \
    n = strchr(line, ';') - strlen(key) - line; \
    song->target = (char*) malloc(sizeof(char)*(n+1)); \
    strncpy(song->target, line + strlen(key), n);

SMSong* SMSong_load(char* filename){


    FILE* fp = fopen(filename, "rt");

    if(fp == NULL){
        return NULL;
    }

    SMSong* song = (SMSong*) malloc(sizeof(SMSong));

    char line[1024];

    while(fgets(line, 1024, fp) != NULL){

        BEGINPARSE();
        PARSE("#TITLE:", title);
        PARSE("#SUBTITLE:", subtitle);
        PARSE("#ARTIST:", artist);
        PARSE("#TITLETRANSLIT:", titletranslit);
        PARSE("#SUBTITLETRANSLIT:", subtitletranslit);
        PARSE("#ARTISTTRANSLIT:", artisttranslit);
        PARSE("#CREDIT:", credit);
        PARSE("#BANNER:", banner);
        PARSE("#BACKGROUND:", background);
        PARSE("#LYRICSPATH:", lyricspath);
        PARSE("#CDTITLE:", cdtitle);
        PARSE("#MUSIC:", music);
        PARSE("#OFFSET:", offset_in_ms);
        /*
        PARSE("#SAMPLESTART:", samplestart_in_ms);
        PARSE("#SAMPLELENGTH:", samplelength_in_ms);
        PARSE("#SELECTABLE:", selectable);
        PARSE("#BPMS:", bpms);
        PARSE("#STOPS:", stops);
        PARSE("#BGCHANGES:", bgchanges);
        */
        ENDPARSE();
    }

    return song;
}
