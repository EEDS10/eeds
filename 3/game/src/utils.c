#include <stdio.h>
#include <sys/time.h>
#include "bmp_read.h"
#include "utils.h"

struct timeval tv;

long gettime(){

    gettimeofday(&tv, NULL);
    
    long output = tv.tv_sec * 1000;
    output += tv.tv_usec  / 1000;

    return output;
}


int strbeginswith(char* str, char *beginswith){
    int i = 0;
    while(str[i] == beginswith[i]){
        if(str[i+1] == '\0' || beginswith[i+1] == '\0'){
            return 1;
        }
        i++;
    }
    return 0;
}


void eeds_blit(bitmap_t* destination, bitmap_t* source, int dx, int dy, int sx, int sy, int w, int h){
    for(int i=MAX(dx,0);i<MIN(dx+w, 320-1);i++){
        for(int j=MAX(dy,0);j<MIN(dy+h, 240-1);j++){
            destination->bitmap[j][i].blue = source->bitmap[j][i].blue;
            destination->bitmap[j][i].red = source->bitmap[j][i].red;
            destination->bitmap[j][i].green = source->bitmap[j][i].green;
        }
    }
}

void eeds_blit_to_allegro(unsigned char** destination, bitmap_t* source, int dx, int dy, int sx, int sy, int w, int h){
    int si = sy;
    int sj = sx;
    for(int i=MAX(dx,0);i<MIN(dx+w, 320-1);i++){
        sj = sx;
        for(int j=MAX(dy,0);j<MIN(dy+h, 240-1);j++){
            destination[j][i*4+0] = source->bitmap[sj][si].blue;
            destination[j][i*4+1] = source->bitmap[sj][si].green; //green
            destination[j][i*4+2] = source->bitmap[sj][si].red; // red
            destination[j][i*4+3] = 0;
            sj++;
        }
        si++;
    }
}
