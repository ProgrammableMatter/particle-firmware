/**
 * @author Raoul Rubien 2015
 */

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * pin on port A change interrupt request
 */
ISR(PCINT0_vect) {
    // rx-A or rx-B
}

/**
 * External Pin, Power-on Reset, Brown-Out Reset, Watchdog Reset
 */
ISR(_VECTOR(0)) {
}

/**
 * Watchdog Time-out
 */
ISR(WDT_vect) {
}

/**
 * Timer/Counter1 Input Capture
 */
ISR(TIM1_CAPT_vect) {
}

/**
 * Timer/Counter1 Compare Match A
 */
ISR(TIM1_COMPA_vect) {
}

/**
 * Timer/Counter1 Compare Match B
 */
ISR(TIM1_COMPB_vect) {
}
/**
 * Timer/Counter1 Overflow - 16 bit counter
 */
ISR(TIM1_OVF_vect) {
}

/**
 * Timer/Counter0 Compare Match A
 */
ISR(TIM0_COMPA_vect) {
}

/**
 * Timer/Counter0 Compare Match B
 */
ISR(TIM0_COMPB_vect) {
}

/**
 * Timer/Counter0 Overflow - 8bit counter
 */
ISR(TIM0_OVF_vect) {
}
