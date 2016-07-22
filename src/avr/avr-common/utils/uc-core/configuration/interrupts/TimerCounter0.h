/**
 * @author Raoul Rubien 20.07.2016
 *
 * Timer / counter 0 related configuration.
 */

#pragma once

#include <avr/interrupt.h>
#include "TimerCounter.h"

#if  defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)
#  if defined(__AVR_ATmega16__)
#    define __TIMER0_INTERRUPT_WAVE_GENERATION_MODE_PWM_PHASE_CORRECT_SETUP \
    (TCCR0 setBit bit(WGM00)); \
    (TCCR0 unsetBit bit(WGM01))

#    define __TIMER0_INTERRUPT_PRESCALER_ENABLE(prescaler) \
    (TCCR0 setBit (prescaler))

#    define __TIMER0_INTERRUPT_PRESCALER_DISABLE \
    (TCCR0 unsetBit(__TIMER_COUNTER_PRESCALER_DISCONNECTED_FLAGS))

#    define __TIMER0_INTERRUPT_OUTPUT_MODE_DISCONNECTED_SETUP \
    (TCCR0 unsetBit ((1 << COM01) | (1 << COM00)))

#    define __TIMER0_COMPARE_INTERRUPT_ENABLE \
    (TIMSK setBit bit(OCIE0))

#    define __TIMER0_COMPARE_INTERRUPT_DISABLE \
    (TIMSK unsetBit bit(OCIE0))

#    define __TIMER0_INTERRUPT_CLEAR_PENDING_COMPARE \
    (((TIFR & (1 << OCF0A)) != 0) ? TIFR = (1 << OCF0) : 0)

#    define __TIMER0_INTERRUPT_COMPARE_VALUE_SETUP(compareValue) \
    (OCR0 = compareValue)

#  elif defined(__AVR_ATtiny1634__)
#    define __TIMER0_INTERRUPT_WAVE_GENERATION_MODE_PWM_PHASE_CORRECT_SETUP \
    (TCCR0A setBit bit(WGM00)); \
    (TCCR0A unsetBit bit(WGM01)); \
    (TCCR0A unsetBit bit(WGM02))

#    define __TIMER0_INTERRUPT_OUTPUT_MODE_DISCONNECTED_SETUP \
    (TCCR0A unsetBit ((1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0)))

#    define __TIMER0_INTERRUPT_PRESCALER_ENABLE(prescaler) \
    (TCCR0A setBit (prescaler))

#    define __TIMER0_INTERRUPT_PRESCALER_DISABLE \
    (TCCR0A unsetBit(__TIMER_COUNTER_PRESCALER_DISCONNECTED_FLAGS))

#    define __TIMER0_COMPARE_INTERRUPT_ENABLE \
    (TIMSK setBit bit(OCIE0A))

#    define __TIMER0_COMPARE_INTERRUPT_DISABLE \
    (TIMSK unsetBit bit(OCIE0A))

#    define __TIMER0_INTERRUPT_CLEAR_PENDING_COMPARE \
    (((TIFR & (1 << OCF0A)) != 0) ? TIFR = (1 << OCF0A) : 0)

#    define __TIMER0_INTERRUPT_COMPARE_VALUE_SETUP(compareValue) \
    (OCR0A = compareValue)

#  endif

#  define __TIMER0_INTERRUPT_TIMER_VALUE_SETUP(timerValue) \
    (TCNT0 = timerValue)

#  define __TIMER0_OVERFLOW_INTERRUPT_ENABLE \
    (TIMSK setBit bit(TOIE0))

#  define __TIMER0_OVERFLOW_INTERRUPT_DISABLE \
    (TIMSK unsetBit bit(TOIE0))
#else
#  error
#endif
