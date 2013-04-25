#include <stdio.h>
#include "leds.h"


void set_leds(unsigned char state){
    printf("setting leds \n");
    FILE* leds = fopen("/dev/leds", "w");
    fputc(state, leds);
    fclose(leds);
    printf("done leds \n");
}
