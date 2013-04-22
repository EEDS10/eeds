#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bmp_read.h"
#include "utils.h"

bitmap_t* eeds_create_bitmap(int w, int h){
    bitmap_t *bitmap = malloc(sizeof(*bitmap));
    bitmap->width = w;
    bitmap->height = h;
    bitmap->bitmap = malloc(sizeof(*bitmap->bitmap) * bitmap->height);
    for (int i = 0; i < bitmap->height; i++) {
        bitmap->bitmap[i] = malloc(sizeof(**bitmap->bitmap) * bitmap->width);
    }
    return bitmap;
}

bitmap_t *eeds_load_bmp(FILE *bmp) {
    int i, x, y, w, h, bytes;
    unsigned char *file_buffer;
    unsigned char header[54];
    bitmap_t* bitmap;

    fread(header, sizeof(*header), 54, bmp);

    w = *((int *) &(header[18]));
    h = abs(*((int *) &(header[22])));

    bitmap = eeds_create_bitmap(w, h);

    bytes = bitmap->width * bitmap->height * 4;
    file_buffer = malloc(sizeof(*file_buffer) * bytes);
    fread(file_buffer, sizeof(*file_buffer), bytes, bmp);

    for (i = 0, x = 0, y = 0; i < bytes;) {
        bitmap->bitmap[bitmap->height - 1 - x][y].blue = file_buffer[i++] & 255;
        bitmap->bitmap[bitmap->height - 1 - x][y].green = file_buffer[i++] & 255;
        bitmap->bitmap[bitmap->height - 1 - x][y].red = file_buffer[i++] & 255;
        i++;

        if (++y == bitmap->width) {
            x++;
            y = 0;
        }
    }

    free(file_buffer);

    return bitmap;
}

void eeds_free_bitmap(bitmap_t *bitmap) {
    for (int i = 0; i < bitmap->height; i++) {
        free(bitmap->bitmap[i]);
    }

    free(bitmap->bitmap);
    free(bitmap);
}


void eeds_render_bitmap(bitmap_t* bitmap, unsigned char** screen, int x, int y){
    int offset_x = MAX(x,0);
    int offset_y = MAX(y,0);
    for(int i=MAX(x,0);i<MIN(x+bitmap->width-1, 320-1);i++){
        for(int j=MAX(y,0);j<MIN(y+bitmap->height-1, 240-1);j++){
            colour_t c = bitmap->bitmap[j-offset_y][i-offset_x];
            if(c.blue == 255 && c.red == 255) continue;
            screen[j][i*4+0] = c.blue;
            screen[j][i*4+1] = c.green; //green
            screen[j][i*4+2] = c.red; // red
            screen[j][i*4+3] = 0;
        }
    }
}
