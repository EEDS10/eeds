#ifndef ALLEGRO_SHIM_H
#define ALLEGRO_SHIM_H
#ifdef NO_ALLEGRO

#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
    #include <fcntl.h>

#include "allegro_shim.h"

FILE* keyboard;
int key[8];
unsigned char* screen;

void install_keyboard() {
    keyboard = fopen("/dev/buttons", "rb");
}

void clear_keybuf() {
    rewind(keyboard);
    int keystates = fgetc(keyboard);

    for(int i=0;i<8;i++){
        key[i] = !!(keystates & (int)pow(2,i));
    }

    /*
    key[0] = !!(keystates & 128);
    key[1] = !!(keystates & 64);
    key[2] = !!(keystates & 32);
    key[3] = !!(keystates & 16);
    */
}


void allegro_init(){
    int screen_size = 320*240*3;
    int framebuffer;
    if(!(framebuffer = open("/dev/fb0", O_RDWR))){
        printf("Failed to open framebuffer.\n"); 
    }
    screen = mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, framebuffer, 0);
    if((int)screen == -1){
        printf("Failed to mmap the framebuffer.\n");
    }
}

#endif
#endif
