#include <stdint.h>
#include <stdlib.h>

#include "synth.h"

sound_t *get_sound(sound_t* sound, waveform_t waveform, int frequency, int attack_duration, int decay_duration, int sustain_duration, int release_duration, int sustain_level) {

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

playback_t *prepare_playback(playback_t *playback, sound_t *sound, int16_t sample_frequency, int16_t master_volume) {

    playback->sound = sound;
    playback->sample_frequency = sample_frequency;
    playback->master_volume = master_volume;
    playback->t = 0;

    playback->ticks_per_period = sample_frequency / sound->frequency;

    return playback;
}

void free_playback(playback_t *playback) {
    free(playback);
    /* What, did you expect me to do the same joke twice? */
}

void reset_playback(playback_t *playback) {
    playback->t = 0;
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

    /* Milliseconds we are into playing the sound. */
    int ms;

    /*
     * Note that we currently use +/- ticks_per_period as the extreme values
     * while working on the sample to simplify generation of the sawtooth
     * waves.
     */
    switch (playback->sound->waveform) {
        case NOISE:
            sample = (rand() % (2 * playback->ticks_per_period)) - playback->ticks_per_period;
            break;
        case SAWTOOTH:
            sample = playback->t % (playback->ticks_per_period * 4);
            sample -= (playback->ticks_per_period/2);
            sample *= 2;
            break;
        case SQUARE:
            if ((playback->t/(playback->ticks_per_period*2)) % 2) {
                sample = playback->ticks_per_period;
            } else {
                sample = -playback->ticks_per_period;
            }
            break;
    }

    ms = (playback->t * 1000) / playback->sample_frequency;
    if (ms <= playback->sound->attack_time) {
        /* Ramp up from no volume to full in attack_time ms. */
        sample = (sample * ms) / playback->sound->attack_time;
    } else if (ms <= playback->sound->decay_time) {
        /* Ramp down from full volume to the sustain volume. */
        sample = (sample * (100 - (((100 - playback->sound->sustain_level) * (ms - playback->sound->attack_time)) / (playback->sound->decay_time - playback->sound->attack_time)))) / 100;
    } else if (ms <= playback->sound->sustain_time) {
        /* Here we just have to stay at the sutain volume. */
        sample = (sample * playback->sound->sustain_level) / 100;
    } else if (ms <= playback->sound->release_time) {
        /*
         * Fade out from sustain volume to "silence" (there's still noise from
         * the DAC).
         */
        sample = (sample * (playback->sound->sustain_level - (((playback->sound->sustain_level) * (ms - playback->sound->sustain_time)) / (playback->sound->release_time - playback->sound->sustain_time)))) / 100;
    } else {
        /* We're finished playing. */
        sample = 0;
    }

    playback->t++;
    return (sample * playback->master_volume) / (playback->ticks_per_period);
}
