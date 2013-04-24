#include <stdlib.h>
#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif
#include "Font.h"
#include "bmp_read.h"
#include "utils.h"


Font* Font_load(char* filename, int char_w, int char_h){
    Font* font = (Font*) malloc(sizeof(Font));
    FILE* fp = fopen(filename, "rb");
    font->bmp = eeds_load_bmp(fp);
    font->char_w = char_w;
    font->char_h = char_h;
    return font;
}

int chr_to_xpos(char c){
    return (((unsigned int)c) - 32) / 15;
}

int chr_to_ypos(char c){
    return (((unsigned int)c) - 32) % 15;
}

void Font_render(Font*font, bitmap_t* screen, char* string, int x, int y){
    do{
        int x_offset = chr_to_xpos(string[0]) * font->char_h;
        int y_offset = chr_to_ypos(string[0]) * font->char_w;
        eeds_blit(font->bmp, screen, x, y, x_offset, y_offset, font->char_w - 6, font->char_h);
        x += font->char_w - 8;
    }while((++string)[0] != '\0');
}
