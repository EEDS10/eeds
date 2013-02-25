/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 * Headerfil
 *
 *****************************************************************************/

#ifndef OEVING_2_H /* vanlig måte å unngå headerrekursjon på */
#define OEVING_2_H

/* legg andre "includes" her, sammen med "defines" og prototyper */
#define ABDAC_INT_LEVEL 0
#define BUTTONS_INT_LEVEL 0

/* prototyper */
int main(int argc, char *argv[]);
static void init_intc(void);
static void init_buttons(void);
static void init_leds(void);
static void init_audio(void);
static void init_hardware(void);

static void button_isr(void);
static void abdac_isr(void);

#endif
