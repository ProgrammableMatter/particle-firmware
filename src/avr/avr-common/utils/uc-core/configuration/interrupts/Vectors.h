/**
 * @author Raoul Rubien
 * 20.07.2016
 */
#pragma once

#include <avr/interrupt.h>

/**
 * Interrupt vectors renamed for convenience.
 */

#define RESET_VECT _VECTOR(0)

#if defined(__AVR_ATtiny1634__)
#  define NORTH_PIN_CHANGE_INTERRUPT_VECT \
    PCINT1_vect

#  define EAST_PIN_CHANGE_INTERRUPT_VECT \
    PCINT0_vect

#  define SOUTH_PIN_CHANGE_INTERRUPT_VECT \
    PCINT2_vect

#  define ACTUATOR_PWM_INTERRUPT_VECT \
    TIM0_COMPA_vect

#  define __UNUSED_TIMER0_OVERFLOW_INTERRUPT_VECT \
    TIMER0_OVF_vect

#  define __UNUSED_TIMER1_OVERFLOW_INTERRUPT_VECT \
    TIMER1_OVF_vect

#elif defined(__AVR_ATmega16__)
#  define NORTH_PIN_CHANGE_INTERRUPT_VECT \
    INT2_vect

#  define EAST_PIN_CHANGE_INTERRUPT_VECT \
    INT1_vect

#  define SOUTH_PIN_CHANGE_INTERRUPT_VECT \
    INT0_vect

#  define ACTUATOR_PWM_INTERRUPT_VECT \
    TIMER0_COMP_vect

#  define __UNUSED_TIMER0_OVERFLOW_INTERRUPT_VECT \
    TIMER0_OVF_vect

#  define __UNUSED_TIMER1_OVERFLOW_INTERRUPT_VECT \
    TIMER1_OVF_vect

#else
#  error
#endif

#if defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)
#  define TX_TIMER_INTERRUPT_VECT \
    TIMER1_COMPA_vect

#  define LOCAL_TIME_INTERRUPT_VECT \
    TIMER1_COMPB_vect
#endif
