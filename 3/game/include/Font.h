#ifndef FONT_H
#define FONT_H

#include "bmp_read.h"

typedef struct Font {
    bitmap_t* bmp;
    int char_w;
    int char_h;
} Font;


Font* Font_load(char* filename, int char_w, int char_h);
void Font_render(Font*font, int* screen, int x, int y, char* string);

#endif
