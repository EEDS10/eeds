#ifndef ALLEGRO_SHIM_H
#define ALLEGRO_SHIM_H

#include "bmp_read.h"

extern int key[8];
extern unsigned char* screen;

#define BITMAP bitmap_t

#define blit eeds_blit

#define  set_color_depth(a);
#define  set_gfx_mode(a, b, c, d, e);
#define END_OF_MAIN()

#define bmp_select(a) 


#define clear_to_color eeds_clear_to_color
#define makecol(a,b,c) (a),(b),(c)
#define blit_to_screen eeds_blit_to_screen

#define KEY_ESC 0

#define KEY_UP 1
#define KEY_LEFT 1

#define KEY_DOWN 2
#define KEY_RIGHT 2

#define KEY_SPACE 3
#define KEY_ENTER 3

#define KEY_A 7
#define KEY_S 5
#define KEY_D 3
#define KEY_F 1

#define GFX_AUTODETECT_WINDOWED 1

#define create_bitmap(x,y) eeds_create_bitmap(x,y)

void install_keyboard();
void clear_keybuf();
void allegro_init();

#endif
