/**
 * @author Raoul Rubien 20.07.2016
 *
 * Timer / counter 1 related configuration.
 */

#pragma once

#include <avr/interrupt.h>
#include "TimerCounter.h"

#if  defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)

#  define __TIMER1_INTERRUPT_CLEAR_PENDING_COMPARE_A \
    (((TIFR & (1 << OCF1A)) != 0) ? TIFR = (1 << OCF1A) : 0)

#  define __TIMER1_INTERRUPT_CLEAR_PENDING_COMPARE_B \
    (((TIFR & (1 << OCF1B)) != 0) ? TIFR = (1 << OCF1B) : 0)

#  define __TIMER1_INTERRUPT_CLEAR_PENDING \
    __TIMER1_INTERRUPT_CLEAR_PENDING_COMPARE_A; \
    __TIMER1_INTERRUPT_CLEAR_PENDING_COMPARE_B

#  define __TIMER1_INTERRUPT_OUTPUT_MODE_DISCONNECTED_SETUP \
    (TCCR1A unsetBit ((1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0)))

#  define __TIMER1_INTERRUPT_WAVE_GENERATION_MODE_CTC_SETUP \
    TCCR1A unsetBit ((1 << WGM11) | (1 << WGM10)); \
    TCCR1B unsetBit ((1 << WGM13) | (1 << WGM12)); \
    TCCR1B setBit ((1 << WGM12))

#  define __TIMER1_INTERRUPT_WAVE_GENERATION_MODE_NORMAL_SETUP \
    TCCR1A unsetBit ((1 << WGM11) | (1 << WGM10)); \
    TCCR1B unsetBit ((1 << WGM13) | (1 << WGM12))

#  define __TIMER1_INTERRUPT_TIMER_VALUE_SETUP(timerValue) \
    (TCNT1 = timerValue)

#  define __TIMER1_INTERRUPT_COMPARE_VALUE_A_SETUP(compareValue) \
    (OCR1A = (compareValue))

#  define __TIMER1_INTERRUPT_COMPARE_VALUE_B_SETUP(compareValue) \
    (OCR1B = (compareValue))

#  define __TIMER1_INTERRUPT_PRESCALER_ENABLE(prescaler) \
    (TCCR1B setBit(prescaler))

#  define __TIMER1_INTERRUPT_PRESCALER_DISABLE \
    (TCCR1B unsetBit(__TIMER_COUNTER_PRESCALER_DISCONNECTED_FLAGS))

#  define __TIMER1_COMPARE_A_INTERRUPT_ENABLE \
    (TIMSK setBit bit(OCIE1A))

#  define __TIMER1_COMPARE_A_INTERRUPT_DISABLE \
    (TIMSK unsetBit bit(OCIE1A))

#  define __TIMER1_COMPARE_B_INTERRUPT_ENABLE \
    (TIMSK setBit bit(OCIE1B))

#  define __TIMER1_COMPARE_B_INTERRUPT_DISABLE \
    (TIMSK unsetBit bit(OCIE1B))

#  define __TIMER1_OVERFLOW_INTERRUPT_ENABLE \
    (TIMSK setBit bit(TOIE1))

#  define __TIMER1_OVERFLOW_INTERRUPT_DISABLE \
    (TIMSK unsetBit bit(TOIE1))

#else
#  error
#endif
