/**
 * @author Raoul Rubien
 * 23.11.16
 */

#ifndef __VECTORS_INTERRUPTS_H
#define __VECTORS_INTERRUPTS_H

//#include <avr/interrupt.h>

/**
 * Interrupt vectors renamed for convenience.
 */
#ifdef __AVR_ATtiny1634__
#  define NORTH_PIN_CHANGE_INTERRUPT_VECT PCINT1_vect
#  define EAST_PIN_CHANGE_INTERRUPT_VECT PCINT0_vect
#  define SOUTH_PIN_CHANGE_INTERRUPT_VECT PCINT2_vect
#  define TX_RX_TIMEOUT_INTERRUPT_VECT TIM0_COMPA_vect
#  define TX_RX_TIMEOUT_OVERFLOW_INTERRUPT TIMER0_OVF_vect
#  define TX_RX_OVERFLOW_INTERRUPT TIMER1_OVF_vect
#else
#  if defined(__AVR_ATmega16__)
#    define NORTH_PIN_CHANGE_INTERRUPT_VECT INT2_vect
#    define EAST_PIN_CHANGE_INTERRUPT_VECT INT1_vect
#    define SOUTH_PIN_CHANGE_INTERRUPT_VECT INT0_vect
#    define TX_RX_TIMEOUT_INTERRUPT_VECT TIMER0_COMP_vect
#    define TX_RX_TIMEOUT_OVERFLOW_INTERRUPT TIMER0_OVF_vect
#    define TX_RX_OVERFLOW_INTERRUPT TIMER1_OVF_vect
#  else
#    error
#  endif
#endif

#if defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)
#  define TX_RX_TIMER_TOP_INTERRUPT_VECT TIMER1_COMPA_vect
#  define TX_TIMER_CENTER_INTERRUPT_VECT TIMER1_COMPB_vect
#endif

#endif
