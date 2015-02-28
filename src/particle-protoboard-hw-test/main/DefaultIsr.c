// @author: Raoul Rubien 2011

#include <avr/io.h>

#include <avr/interrupt.h>

/**
 * pin change interrupt request 2
 */
ISR( PCINT2_vect ) {
	// rx-b
}

/**
 * pin change interrupt request 5
 */
ISR( PCINT5_vect ) {
	// rx-a
}

/**
 * timer0/counter0: 8-bit counter overflow
 */
ISR( TIMER0_OVF_vect ) {
}

/**
 * External Pin, Power-on Reset, Brown-Out Reset, Watchdog Reset
 */
ISR( RESET_vect ) {
}

/**
 * Watchdog Time-out
 */
ISR( WDT_vect ) {
}

/**
 * Timer/Counter1 Input Capture
 */
ISR( TIM1_CAPT_vect ) {
}

/**
 * Timer/Counter1 Compare Match A
 */
ISR( TIM1_COMPA_vect ) {
}

/**
 * Timer/Counter1 Compare Match B
 */
ISR( TIM1_COMPB_vect ) {
}
/**
 * Timer/Counter1 Overflow
 */
ISR( TIM1_OVF_vect ) {
}

/**
 * Timer/Counter0 Compare Match A
 */
ISR( TIM0_COMPA_vect ) {
}

/**
 * Timer/Counter0 Compare Match B
 */
ISR( TIM0_COMPB_vect ) {
}

/**
 * Timer/Counter0 Overflow
 */
ISR( TIM0_OVF_vect ) {
}
