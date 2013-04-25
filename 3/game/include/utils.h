#ifndef UTILS_H
#define UTILS_H

#include "bmp_read.h"
#ifndef NO_ALLEGRO
#include <allegro.h>
#else
#include "allegro_shim.h"
#endif



#ifndef MIN
    #define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
    #define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif


#define FLIP_ENDIANNESS_INT32(a)(((a>>24)&0xff)     | \
                                 ((a<<8)&0xff0000)  | \
                                 ((a>>8)&0xff00)    | \
                                 ((a<<24)&0xff000000))
//#define FLIP_ENDIANNESS_INT32(a) (a)


long gettime();
int strbeginswith(char* str, char *beginswith);
void eeds_blit(bitmap_t* destination, bitmap_t* source, int dx, int dy, int sx, int sy, int w, int h);
void eeds_blit_to_screen(bitmap_t* source, unsigned char* destination, int dx, int dy, int sx, int sy, int w, int h);
void eeds_clear_to_color(bitmap_t* bitmap, int r, int g, int b);

#ifndef NO_ALLEGRO
void blit_to_screen(bitmap_t* source, BITMAP* destination, int dx, int dy, int sx, int sy, int w, int h);
#endif

#endif
