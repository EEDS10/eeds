/******************************************************************************
 *
 * Øving 1 UCSysDes
 *
 *****************************************************************************/

.include "io.s"  /* inkludere noen nyttige symboler (konstanter) */

/* Eksempel på hvordan sette symboler (se også io.s) */
SR_GM =   16  /* statusregisterflag "GM" er bit 16 */

/*****************************************************************************/
/* text-segment */
/* all programkoden må plasseres i dette segmentet */

.text

.globl  _start
_start: /* programutføring vil starte her */
    lddpc r0, per_pioc_peker
    lddpc r2, oer_pioc_peker
    lddpc r3, sodr_pioc_peker
    lddpc r5, codr_pioc_peker
    lddpc r6, per_piob_peker
    lddpc r7, puer_piob_peker
    lddpc r8, pdsr_piob_peker
    mov r4, 0x08 /* 4 holds the paddle */
    mov r1, 0xff
    st.w r0, r1 /* Set PER for all LEDs */
    st.w r2, r1 /* Set OER for all LEDs */
    st.w r6, r1 /* Set PER for all buttons */
    st.w r7, r1 /* Set PUER for all buttons */
    /* st.w r3, r1  Enable the LEDs in R1 */
    st.w r5, r1 /* Disable the LEDs in R1 */

    /* eksempel på lesing fra minneområdet "eksempelvariabel"
    lddpc r0, led_pioc_peker  /* last peker til r0
    ld.w r1, r0  /* last fra minnet til r1 */

loop:   /* evig løkke */
    st.w r5, r1 /* turns all LEDs off*/
    rcall read_buttons
    cp.w r12, 0x1
    brne else_if /* branches to else_if if button 0 was not pushed */
    rcall move_paddle_right
    rjmp end_if

else_if:
    cp.w r12, 0x4
    brne end_if /* branches to end_if if button 2 was not pressed */
    rcall move_paddle_left

end_if:
    st.w r3, r4
    rjmp loop


read_buttons:
    ld.w r12, r8
    eor r12, r1 /* == not R12, for the first 8 bits */
    and r12, r1 /* to ensure the first 8 bits remain untouched while the last 8 are set to zero */
    ret r12


move_paddle_right: /*moves the 'paddle' right by right rotating the value in r4 */
    cp.w r4, 0x01
    breq move_paddle_right_reset
    lsr r4, 0x1
    rjmp move_paddle_right_end

move_paddle_right_reset:
    mov r4, 0x80

move_paddle_right_end:
    ret r12


move_paddle_left: /* moves the 'paddle' left by left rotating the value in r4 */
    cp.w r4, 0x80
    breq move_paddle_left_reset
    lsl r4, 0x1
    rjmp move_paddle_left_end

move_paddle_left_reset:
    mov r4, 0x01

move_paddle_left_end:
    ret r12

oer_pioc_peker:
    .int AVR32_PIOC|AVR32_PIO_OER
per_pioc_peker:
    .int AVR32_PIOC|AVR32_PIO_PER
sodr_pioc_peker: /* Turn LEDs on */
    .int AVR32_PIOC|AVR32_PIO_SODR
codr_pioc_peker: /* Turn LEDs off */
    .int AVR32_PIOC|AVR32_PIO_CODR
per_piob_peker:
    .int AVR32_PIOB|AVR32_PIO_PER
puer_piob_peker: /* Pull-up resistors */
    .int AVR32_PIOB|AVR32_PIO_PUER
pdsr_piob_peker: /* To read button status */
    .int AVR32_PIOB|AVR32_PIO_PDSR

/*****************************************************************************/
/* data-segment */
/* alle dataområder som skal kunne skrives til må plasseres her */

.data

