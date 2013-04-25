#ifndef ALLEGRO_SHIM_H
#define ALLEGRO_SHIM_H
#ifdef NO_ALLEGRO

#include <stdio.h>

#include "allegro_shim.h"

void install_keyboard() {
    keyboard = fopen("/dev/buttons", "rb");
}

void clear_keybuf() {
    int keystates = fgetc(keyboard);

    key[0] = !!(keystates & 128);
    key[1] = !!(keystates & 64);
    key[2] = !!(keystates & 32);
    key[3] = !!(keystates & 16);
}

#endif
#endif
