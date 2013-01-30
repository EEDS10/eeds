/******************************************************************************
 *
 * Exercise 1 EEDS
 *
 * by emiltayl, trondrud and sigveseb
 *
 *****************************************************************************/

/************************
 * INCLUDES AND DEFINES *
 ************************/

/* include various convenient symbols (constants) */
.include "io.s"

/* Status register flag "GM" is bit 16 */
SR_GM =   16  

/* led masks */
LED_0 = 0x1
LED_1 = 0x2
LED_2 = 0x4
LED_3 = 0x8
LED_4 = 0x10
LED_5 = 0x20
LED_6 = 0x40
LED_7 = 0x80
ALL_LEDS = 0xff

/* button masks */
SW_0 = 0x1
SW_1 = 0x2
SW_2 = 0x4
SW_3 = 0x8
SW_4 = 0x10
SW_5 = 0x20
SW_6 = 0x40
SW_7 = 0x80

/* change this to configure debounce length */
DEBOUNCE = 0xfff

/* change this to configure paddle start position */
PADDLE_START_POSITION = LED_3

/* #yolo */
NULL = 0




/****************
 * DATA SEGMENT *
 ****************/
.text

/* main entry point */
.globl  _start
_start:

    /********
     * INIT *
     ********/

    /* load pointers to piob and pioc */
    lddpc r2, piob_offset
    lddpc r3, pioc_offset
    lddpc r7, intc_base
    mov r8, button_interrupt_routine

    /* set 0 in r1, as per convention */
    mov r1, NULL

    /* r4 holds the paddle */
    mov r4, PADDLE_START_POSITION

    /* set up some nice constants */
    mov r6, ALL_LEDS
    mov r5, SW_0 | SW_2

    /* enable IO pins for the LEDs */
    st.w r3[AVR32_PIO_PER], r6

    /* set the IO pins to be outputs */
    st.w r3[AVR32_PIO_OER], r6

    /* enable IO pins for the buttons */
    st.w r2[AVR32_PIO_PER], r6

    /* enable pull-up resistors */
    st.w r2[AVR32_PIO_PUER], r6

    /* defensively turn off all interrupts */
    st.w r2[AVR32_PIO_IDR], r6

    /* turn on button interrupts for SW0 and SW2 */
    st.w r2[AVR32_PIO_IER], r5 

    /* set the EVBA to 0, as specified in the compendium */
    mtsr 4, r1

    /* set button_interrupt_routine to handle button interrupts */
    st.w r7[AVR32_INTC_IPR14], r8

    /* finally, enable interrupts! */
    csrf SR_GM


    

/*************
 * MAIN LOOP *
 *************/
loop:

    /* set the leds to the new state */
    rcall set_leds

    /* chill-out zone */
    sleep 1

    /* when we break out of the sleep, a button interrupt was
    just handled, and r12 contains the state of the buttons */

    /* if SW0 was pressed */
    cp.w r12, SW_0
    brne else_if
        /* move paddle to the right */
        rcall move_paddle_right
        rjmp end_if

    /* else if SW2 was pressed */
    else_if:
        cp.w r12, SW_2
        brne end_if
        /* move paddle to the left */
        rcall move_paddle_left

    /* end if */
    end_if:

    /* go back to the top of the loop */
    rjmp loop


/*
 * Subroutine that turns on the LED the paddle is at.
 * The paddle's state is in r4.
 */
set_leds:

    /* turns all LEDs off*/
    st.w r3[AVR32_PIO_CODR], r6

    /* turn on the appropriate LED */
    st.w r3[AVR32_PIO_SODR], r4

    /* return */
    ret r12


/*
 * Interrupt routine for handling button presses. It reads
 * the button status, debounces, notifies that the interrupt
 * has been handled, and returns the button state in r12.
 */
button_interrupt_routine:

    /* read button state */
    rcall read_buttons

    /* software debounce to stop button glitching */
    rcall debounce 

    /* notify that the interrupt has been handled */
    ld.w r0, r2[AVR32_PIO_ISR]

    /* return */
    rete



/*
 * Debounce routine. It loops for a while, then returns.
 * See fig 2.9 in the compendium.
 */
debounce:
    mov r0, DEBOUNCE
    debouce_loop:
        sub r0, 1
        cp.w r0, 0
        brle debouce_loop_end
        rjmp debouce_loop
    debouce_loop_end:
    ret r12
     


/*
 * Read button status routine. Returns button status in r12. 
 * Note: button status is inverted to make it easier to work with.
 */
read_buttons:
    /* read button status from piob */
    ld.w r12, r2[AVR32_PIO_PDSR]

    /* invert the lower 8 bits of the button status for convenience */
    eor r12, r6
    
    /* mask away everything but the lower 8 bits */
    and r12, r6

    /* cache the button state */
    mov r0, r12

    /* invert old button state */
    eor r7, r6

    /* we only want buttons that are set now, but not in the old state  */
    and r12, r7

    /* save the new button state to be the new old button state */
    mov r7, r12

    /* finally, return button status in r12 */
    ret r12


/*
 * Move paddle right routine. Loops off the end of the board. 
 */
move_paddle_right:
    /* if paddle is not at the far-right end of the board */
    cp.w r4, LED_0
    breq move_paddle_right_reset
        /* shift paddle register to the right */
        lsr r4, 1
        rjmp move_paddle_right_end

    /* else, the paddle is at the end... */
    move_paddle_right_reset:
        /*and we must reset to the far-left end */
        mov r4, LED_7

    /* end if */
    move_paddle_right_end:
    
    /* finally, return */
    ret r12


/*
 * Move paddle left routine. Loops off the end of the board. 
 */
move_paddle_left:
    /* if paddle is not at the far-left end of the board */
    cp.w r4, LED_7
    breq move_paddle_left_reset
        /* shift paddle register to the left */
        lsl r4, 1
        rjmp move_paddle_left_end

    /* else, the paddle is at the end... */
    move_paddle_left_reset:
        /*and we must reset to the far-left end */
        mov r4, LED_0

    /* end if */
    move_paddle_left_end:

    /* finally, return */
    ret r12



/************
 * POINTERS *
 ************/

pioc_offset:
    .int AVR32_PIOC
piob_offset:
    .int AVR32_PIOB
intc_base:
    .int AVR32_INTC

/****************
 * DATA SEGMENT *
 ****************/

.data
