#include <stdio.h>
#include <sys/time.h>
#ifndef NO_ALLEGRO
#include <allegro.h>
#else
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
    int si;
    int sj = sx - MIN(dy, 0);
    int j_max = MIN(dy+h, 240);
    int i_max = MIN(dx+w, 320);
    for(int j=MAX(dy,0);j<j_max;j++){
        si = sy;
        for(int i=MAX(dx,0);i<i_max;i++){
            colour_t* c = &(source->bitmap[sj][si]);
            if(c->blue != 255 || c->red != 255 || c->green != 0){
                destination->bitmap[j][i].blue  = c->blue;
                destination->bitmap[j][i].red   = c->red;
                destination->bitmap[j][i].green = c->green;
            }
            si++;
        }
        sj++;
    }
}

void eeds_clear_to_color(bitmap_t* bitmap, int r, int g, int b){
    for(int j=0;j<bitmap->width;j++){
        for(int i=0;i<bitmap->height;i++){
            bitmap->bitmap[i][j].blue  = b;
            bitmap->bitmap[i][j].red   = r;
            bitmap->bitmap[i][j].green = g;
        }
    }
}

/* hand-optimized and benchmarked */
/* yes, the loop unrolling can drop a pixel here and there, but it is worth it. */
void eeds_blit_to_screen(bitmap_t* source, unsigned char* destination, int dx, int dy, int sx, int sy, int w, int h){
    int k = 0;
    int j_max = sx + MIN(dy+h, 240) - MAX(dy, 0);
    int i_max = sy + MIN(dx+w, 320) - MAX(dx, 0) - 1;
    for(int j=sx;j<j_max;j++){
        for(int i=sy;i<i_max;i+=2){
            colour_t* c = &(source->bitmap[j][i]);
            destination[k] = c->blue;
            destination[k+1] = c->green;
            destination[k+2] = c->red;
            c = &(source->bitmap[j][i+1]);
            destination[k+3] = c->blue;
            destination[k+4] = c->green;
            destination[k+5] = c->red;
            k += 6;
        }
    }
}


#ifndef NO_ALLEGRO
void blit_to_screen(bitmap_t* source, BITMAP* destination, int dx, int dy, int sx, int sy, int w, int h){
    int si = sy;
    int sj = sx;
    for(int j=MAX(dy,0);j<MIN(dy+h, 240);j++){
        si = sy;
        for(int i=MAX(dx,0);i<MIN(dx+w, 320);i++){
            colour_t c = source->bitmap[sj][si];
                putpixel(destination, si, sj, makecol(c.red, c.green, c.blue));
            si++;
        }
        sj++;
    }
}
#endif
