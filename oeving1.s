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

/*
 * r0: pioc
 * r1: 0x00
 * r2: piob
 * r4: current light position
 * r5: 0xff
 * r6: 0x05
 * r7: intc
 * r8: button_interrupt_routine
*/
.text

.globl  _start
_start: /* programutføring vil starte her */
    lddpc r0, pioc_offset
    lddpc r2, piob_offset
    lddpc r7, intc_base
    mov r8, button_interrupt_routine
    mov r1, 0x00
    mov r4, 0x08 /* 4 holds the paddle */
    mov r5, 0xff
    mov r6, 0x05
    st.w r0[AVR32_PIO_PER], r5 /* Set PER for all LEDs */
    st.w r0[AVR32_PIO_OER], r5 /* Set OER for all LEDs */
    st.w r2[AVR32_PIO_PER], r5 /* Set PER for all buttons */
    st.w r2[AVR32_PIO_PUER], r5 /* Set PUER for all buttons */
    st.w r0[AVR32_PIO_OER], r5 /* Disable the LEDs in R1 */
    st.w r2[AVR32_PIO_IDR], r5
    st.w r2[AVR32_PIO_IER], r6 /* Turn on interrupts for button 0 and 2 */
    mtsr 4, r1
    st.w r7[AVR32_INTC_IPR14], r8
    csrf SR_GM

loop:   /* evig løkke */
    rjmp loop

button_interrupt_routine:
    st.w r0[AVR32_PIO_CODR], r5 /* turns all LEDs off*/
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
    st.w r0[AVR32_PIO_SODR], r4 /* Enable the LED that should be enabled */
    ld.w r8, r2[AVR32_PIO_ISR]
    rete



read_buttons:
    ld.w r12, r2[AVR32_PIO_PDSR]
    eor r12, r5 /* == not R12, for the first 8 bits */
    and r12, r5 /* to ensure the first 8 bits remain untouched while the last 8 are set to zero */
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

/*
button_interrupt_routine:
    rcall read_buttons
    ld.w r8, r2[AVR32_PIO_ISR]
    rete
*/

pioc_offset:
    .int AVR32_PIOC
piob_offset:
    .int AVR32_PIOB
intc_base:
    .int AVR32_INTC

/*****************************************************************************/
/* data-segment */
/* alle dataområder som skal kunne skrives til må plasseres her */

.data
