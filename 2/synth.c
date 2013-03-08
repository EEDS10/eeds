#include <stdint.h>
#include <stdlib.h>

#include "synth.h"

sound_t *get_sound(waveform_t waveform, int frequency, int attack_duration, int decay_duration, int sustain_duration, int release_duration, int sustain_level) {
    sound_t *sound = malloc(sizeof(*sound));

    sound->waveform = waveform;
    sound->frequency = frequency;
    sound->attack_time = attack_duration;
    sound->decay_time = sound->attack_time + decay_duration;
    sound->sustain_time = sound->decay_time + sustain_duration;
    sound->release_time = sound->sustain_time + release_duration;
    sound->sustain_level = sustain_level;

    return sound;
}

void free_sound(sound_t *sound) {
    free(sound);
    /* What did you expect? */
}

playback_t *prepare_playback(sound_t *sound, int16_t sample_frequency, int16_t master_volume) {
    playback_t *playback = malloc(sizeof(*playback));

    playback->sound = sound;
    playback->sample_frequency = sample_frequency;
    playback->master_volume = master_volume;
    playback->t = 0;

    playback->ticks_per_period = sample_frequency / sound->frequency;

    return playback;
}

playback_t free_playback(playback_t *playback) {
    free(playback);
    /* What, did you expect me to do the same joke twice? */
}

int playback_finished(playback_t *playback) {
    return (playback->t / playback->sample_frequency) >= playback->sound->release_time;
}

int16_t next_sample(playback_t *playback) {
    /*
     * Yes, we're supposed to return a int16_t. However, we want some more bits
     * to play with while generating the sample.
     */
    int sample;

    switch (playback->sound->waveform) {
        case SQUARE:
            if ((playback->t/(playback->ticks_per_period*2)) % 2) {
                sample = playback->ticks_per_period;
            } else {
                sample = -playback->ticks_per_period;
            }
            break;
        case TRIANGLE:
            /* TODO */
            break;
        case SAWTOOTH:
            sample = playback->t % (playback->ticks_per_period * 4);
            sample -= (playback->ticks_per_period/2);
            sample *= 2;
            break;
    }

    playback->t++;
    return (sample * playback->master_volume) / (playback->ticks_per_period);
}
