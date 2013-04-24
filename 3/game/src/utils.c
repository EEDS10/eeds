#include <stdio.h>
#include <sys/time.h>
#ifdef NO_ALLEGRO
#include "allegro_shim.h"
#endif
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

void eeds_blit(bitmap_t* source, bitmap_t* destination, int dx, int dy, int sx, int sy, int w, int h){
    //printf("eeds_blit( destination: %p, source: %p, dx: %i, dy: %i, sx: %i, sy: %i, w: %i, h: %i )\n", destination, source, dx, dy, sx, dy, w, h);
    int si = sy;
    int sj = sx;
    for(int j=MAX(dy,0);j<MIN(dy+h, 240);j++){
        si = sy;
        for(int i=MAX(dx,0);i<MIN(dx+w, 320);i++){
            colour_t c = source->bitmap[sj][si];
            if(c.blue != 255 || c.red != 255){
                destination->bitmap[j][i].blue  = c.blue;
                destination->bitmap[j][i].red   = c.red;
                destination->bitmap[j][i].green = c.green;
            }
            si++;
        }
        sj++;
    }
}

void eeds_clear_to_color(bitmap_t* bitmap, int r, int g, int b){
    for(int j=0;j<bitmap->width;j++){
        for(int i=0;i<bitmap->height;i++){
            bitmap->bitmap[i][j].blue  = 255;
            bitmap->bitmap[i][j].red   = 255;
            bitmap->bitmap[i][j].green = 255;
        }
    }
}

void eeds_blit_to_allegro(bitmap_t* source, unsigned char* destination, int dx, int dy, int sx, int sy, int w, int h){
    //printf("eeds_blit_to_allegro( destination: %p, source: %p, dx: %i, dy: %i, sx: %i, sy: %i, w: %i, h: %i )\n", destination, source, dx, dy, sx, dy, w, h);
    int si = sy;
    int sj = sx;
    int k = 0;
    for(int j=MAX(dy,0);j<MIN(dy+h, 240);j++){
        si = sy;
        for(int i=MAX(dx,0);i<MIN(dx+w, 320);i++){
            destination[ k ] = source->bitmap[sj][si].blue;
            destination[k+1] = source->bitmap[sj][si].green;
            destination[k+2] = source->bitmap[sj][si].red;
            k += 3;
            si++;
        }
        sj++;
    }
}
