#include <stdlib.h>
#include "Font.h"
#include "bmp_read.h"


Font* Font_load(char* filename, int char_w, int char_h){
    Font* font = (Font*) malloc(sizeof(Font));
    FILE* fp = fopen(filename, "rb");
    font->bmp = eeds_load_bmp(fp);
    font->char_w = char_w;
    font->char_h = char_h;
    return font;
}


void Font_render(Font*font, int* screen, char* string, int x, int y){
    do{
        printf("%c", string[0]);
    }while(string++[0] != '\0');
    printf("\n");
}
