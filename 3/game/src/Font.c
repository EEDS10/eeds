#include <stdlib.h>
#include "Font.h"
#include "bmp_read.h"


Font* Font_load(char* filename, int char_w, int char_h){
    Font* font = (Font*) malloc(sizeof(Font));
    FILE* fp = fopen(filename, "rb");
    font->bmp = load_bmp(fp);
    font->char_w = char_w;
    font->char_h = char_h;
    return font;
}


void Font_render(Font*font, int* screen, int x, int y, char* string){
    while(string++ != '\0'){
        printf("%c", string[0]);
    }
}
