typedef enum {SQUARE, TRIANGLE, SAWTOOTH} waveform_t;

typedef struct sound_struct {
    waveform_t waveform;
    int frequency;

    /*
     * All the times are times from the sound starts playing until that part is
     * finished. The times should be given in milliseconds.
     */
    int attack_time;
    int decay_time;
    int sustain_time;
    int release_time;

    /*
     * Percentage of the master volume the sound should sustain at. The variable
     * should be between 0 and 100.
     */
    int sustain_level;
} sound_t;

/* Struct used for playing a sound. */
typedef struct playback_stuct {
    sound_t *sound;
    int16_t sample_frequency;
    int16_t master_volume;
    int t;

    /* Useful variables used to avoid recalculation, do not touch. */
    int ticks_per_period;
} playback_t;

/*
 * Malloc space for a new sound and populate it with the given data. Unlike the
 * struct, here you should just give the various durations, and the function
 * will do all the calculating for you.
 *
 * The function returns a pointer to the allocated sound struct.
 */
sound_t *get_sound(waveform_t waveform, int frequency, int attack_duration, int decay_duration, int sustain_duration, int release_duration, int sustain_level);
void free_sound(sound_t *sound);

/*
 * Malloc space for a new playback_struct and populate it with appropriate
 * values.
 */
playback_t *prepare_playback(sound_t *sound, int16_t sample_frequency, int16_t master_volume);
void free_playback(playback_t *playback);

/* Returns whether or not a playback has finished playing. */
int playback_finished(playback_t *playback);

int16_t next_sample(playback_t *playback);
