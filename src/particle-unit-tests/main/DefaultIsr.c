// @author: Raoul Rubien 2015

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * pin on port A change interrupt request
 */
ISR( INT0_vect ) {
	// rx-A or rx-B
}

/**
 * pin on port B change interrupt request
 */
ISR( INT1_vect ) {
}

/**
 * External Pin, Power-on Reset, Brown-Out Reset, Watchdog Reset
 */
ISR( _VECTOR(0) ) {
}

/**
 * Timer/Counter1 Input Capture
 */
ISR( TIMER1_CAPT_vect ) {
}

/**
 * Timer/Counter1 Compare Match A
 */
ISR( TIMER1_COMPA_vect ) {
}

/**
 * Timer/Counter1 Compare Match B
 */
ISR( TIMER1_COMPB_vect ) {
}

/**
 * Timer/Counter0 Overflow
 */
ISR( TIMER0_OVF_vect ) {
}

/**
 * Timer/Counter1 Overflow
 */
ISR( TIMER1_OVF_vect ) {
}
