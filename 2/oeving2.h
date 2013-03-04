/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 * Headerfil
 *
 *****************************************************************************/

#ifndef OEVING_2_H
#define OEVING_2_H

int main(int argc, char *argv[]);
static void init_intc(void);
static void init_hardware(void);

static void button_isr(void);
static void abdac_isr(void);
#endif
