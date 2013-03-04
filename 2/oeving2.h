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

/* prototyper */
int main(int argc, char *argv[]);
static void init_intc(void);
static void init_hardware(void);

static void button_isr(void);
static void abdac_isr(void);

static void LEDcounting(int countTo);
#endif
