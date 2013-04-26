#include <stdio.h>
#include "leds.h"

void eeds_set_leds(unsigned char state){
    FILE* leds = fopen("/dev/leds", "w");
    fputc(state, leds);
    fclose(leds);
}
