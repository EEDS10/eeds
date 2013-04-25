#ifndef FONT_H
#define FONT_H

#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif
#include "bmp_read.h"

typedef struct Font {
    bitmap_t* bmp;
    int char_w;
    int char_h;
} Font;


Font* Font_load(char* filename, int char_w, int char_h);
void Font_render(Font*font, bitmap_t* screen, char* string, int x, int y, int kerning);

#endif
