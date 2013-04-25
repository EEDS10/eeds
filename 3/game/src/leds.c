#include "leds.h"


void set_leds(unsigned char state){
    FILE* leds = fopen("/dev/leds/", "w");
    fwrite(&state, sizeof(unsigned char), 1, leds);
    fclose(leds);
}

void set_led(int n){
    set_leds(1<<n);
}
