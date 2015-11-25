// @author: Raoul Rubien 2015

#include <avr/io.h>
#include <avr/interrupt.h>

#include <uc-core/Globals.h>
#include <common/PortInteraction.h>
#include <uc-core/ParticleIoDefinitions.h>

extern ParticleState GlobalState;


// TODO on INT[1:0]_vect the timer/compare 0 timer must be paused and released afterwards to prevent loosing interrupts
/**
 * pin change interrupt 0 - INT0
 * This interrupt routine is triggered on falling edges of the north reception pin. In other words on received
 * falling edges from the north neighbour.
 */
ISR(INT0_vect) {
    SREG unsetBit bit(SREG_I);
#ifdef __AVR_ATmega16__
    if (GlobalState.northRxEvents == 255) {
        asm("BREAK");
    }
#endif
    GlobalState.northRxEvents++;
    GIFR = bit(INTF0);
}

/**
 * This interrupt routine is triggered on falling edges from the south reception pin. In other words on received
 * falling edged from the south neighbour.
 */
ISR(INT1_vect) {
    SREG unsetBit bit(SREG_I);
#ifdef __AVR_ATmega16__
    if (GlobalState.southRxEvents == 255) {
        asm("BREAK");
    }
#endif
    GlobalState.southRxEvents++;
    GIFR = bit(INTF1);
}

/**
 * timer/counter 0 compare - TIMER0 compare with OCR0
 * This interrupt routine is triggered when the counter equals to OCR0 value.
 */
ISR(TIMER0_COMP_vect) {
    SREG unsetBit bit(SREG_I);
    TCNT0 = 0;

    NORTH_TX_TOGGLE;
    SOUTH_TX_TOGGLE;
    NORTH_TX_TOGGLE;
    SOUTH_TX_TOGGLE;

    TIFR = bit(OCF0);
}

/**
 * Timer/Counter0 Overflow
 */
ISR(TIMER0_OVF_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}




/**
 * Timer/Counter1 Input Capture
 */
ISR(TIMER1_CAPT_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Compare Match A
 */
ISR(TIMER1_COMPA_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Compare Match B
 */
ISR(TIMER1_COMPB_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Overflow
 */
ISR(TIMER1_OVF_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * External Pin, Power-on Reset, Brown-Out Reset, Watchdog Reset
 */
ISR(_VECTOR(0)) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}