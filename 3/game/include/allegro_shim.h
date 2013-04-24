#ifndef ALLEGRO_SHIM_H
#define ALLEGRO_SHIM_H

#include "bmp_read.h"

int key[8];

#define BITMAP bitmap_t

#define blit eeds_blit

#define allegro_init()
#define install_keyboard();
#define  set_color_depth(a);
#define  set_gfx_mode(a, b, c, d, e);
#define END_OF_MAIN()

#define bmp_select(a) 


#define clear_to_color eeds_clear_to_color
#define makecol(a,b,c) (a),(b),(c)
#define blit_to_screen eeds_blit_to_screen

#define KEY_ESC 1
#define KEY_SPACE 2
#define KEY_UP 4
#define KEY_DOWN 8

#define GFX_AUTODETECT_WINDOWED 1

#define clear_keybuf();

#define create_bitmap(x,y) eeds_create_bitmap(x,y)

#endif
