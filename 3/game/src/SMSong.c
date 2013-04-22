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

#define PARSECUSTOM(key) \
    }else if(strbeginswith(line, key)){ \

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

#define PARSELIST(key, target, type, process) \
    }else if(strbeginswith(line, key)){ \
    n = strchr(line, ';') - strlen(key) - line; \
    char* list = (char*) malloc(sizeof(char)*(n+1)); \
    strncpy(list, line + strlen(key), n); \
    char* item = strtok(list, ",="); \
    int n_tokens = 0; \
    while(item != NULL){ \
        item = strtok(NULL, ",="); \
        n_tokens++; \
    } \
    song->target = (type*) malloc(sizeof(type) * n_tokens); \
    item = list; \
    for(int i=0;i<n_tokens;i++){ \
        song->target[i] = process(item); \
        item += strlen(item) + 1; \
    }

void* identity(void*item){
    return item;
}

int intify(char*item){
    return (int) (round(atof(item) * 1000));
}


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
        PARSELIST("#BPMS:", BPMs, int, intify);
        PARSE("#STOPS:", stops);
        PARSELIST("#BGCHANGES:", BGchanges, int, intify);
        PARSECUSTOM("#NOTES:")
            fgets(line, 1024, fp);
         
            /* skip notestype */
            fgets(line, 1024, fp);

            /* skip description */
            fgets(line, 1024, fp);

            /* skip difficultyclass */
            fgets(line, 1024, fp);

            /* skip difficultymeter */
            fgets(line, 1024, fp);

            /* skip radarvalues */
            fgets(line, 1024, fp);

            /* parse notes */
            song->measures = (Measure**) malloc(sizeof(Measure*) * 256);
            int i=0;
            while(line != NULL && line[0] != ';'){
                int j=0;
                song->measures[i] = (Measure*) malloc(sizeof(Measure));
                while(line != NULL && line[0] != ',' && line[0] != ';'){
                    strncpy((char * restrict)song->measures[i]->rows[j++],(char * restrict) strdup(line), 4);
                    fgets(line, 1024, fp);
                }
                song->measures[i]->n_rows = j;
                j = 0;
                i++;
                fgets(line, 1024, fp);
            }

        ENDPARSE();
    }

    return song;
}
