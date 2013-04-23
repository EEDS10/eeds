#ifndef FONT_H
#define FONT_H

#ifdef NO_ALLEGRO
#include <allegro.h>
#endif
#include "bmp_read.h"

typedef struct Font {
    bitmap_t* bmp;
    int char_w;
    int char_h;
} Font;


Font* Font_load(char* filename, int char_w, int char_h);
void Font_render(Font*font, BITMAP* screen, char* string, int x, int y);

#endif
