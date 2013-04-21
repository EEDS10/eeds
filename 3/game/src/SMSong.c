#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "SMSong.h"


#define BEGINPARSE() do{ int n; if(0){
#define ENDPARSE() }}while(0);
#define PARSE(key, target) \
    }else if(strbeginswith(line, key)){ \
    n = strchr(line, ';') - strlen(key) - line; \
    song->target = (char*) malloc(sizeof(char)*(n+1)); \
    strncpy(song->target, line + strlen(key), n);

#define PARSEINT(key, target) \
    }else if(strbeginswith(line, key)){ \
    n = strchr(line, ';') - strlen(key) - line; \
    song->target = (int)round(strtod((char * restrict) line + strlen(key), (char ** restrict) (line + strlen(key) + n)) * 1000);

#define PARSEBOOL(key, target) \
    }else if(strbeginswith(line, key)){ \
    n = strchr(line, ';') - strlen(key) - line; \
    char* temp = (char*) malloc(sizeof(char)*(n+1)); \
    strncpy(temp, line + strlen(key), n); \
    song->target = strcmp(temp, "YES") == 0;



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
        PARSEINT("#OFFSET:", offset_in_ms);
        PARSEINT("#SAMPLESTART:", samplestart_in_ms);
        PARSEINT("#SAMPLELENGTH:", samplelength_in_ms);
        PARSEBOOL("#SELECTABLE:", selectable);
        PARSE("#BPMS:", BPMs);
        PARSE("#STOPS:", stops);
        PARSE("#BGCHANGES:", BGchanges);
        ENDPARSE();
    }

    return song;
}
