#ifndef AUDIO_H
#define AUDIO_H

#include <avr32/ap7000.h>

#define ABDAC_INT_LEVEL 0

void audio_init(volatile avr32_pio_t* pio,
                volatile avr32_pm_t* pm,
                volatile avr32_abdac_t* dac,
                void* handler,
                int IRQ);
int audio_read(void);

#endif
