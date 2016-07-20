/**
 * @author Raoul Rubien
 * 20.07.2016
 */

#pragma once

/**
 * define timer / counter prescaler
 */
#if  defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)

#  define __TIMER_COUNTER_PRESCALER_DISCONNECTED_FLAGS \
    ((1 << CS02) | (1 << CS01) | (1 << CS00))

#  define __TIMER_COUNTER_PRESCALER_1 \
    ((0 << CS02) | (0 << CS01) | (1 << CS00))

#  define __TIMER_COUNTER_PRESCALER_8 \
    ((0 << CS02) | (1 << CS01) | (0 << CS00))

#  define __TIMER_COUNTER_PRESCALER_64 \
  ((0 << CS02) | (1 << CS01) | (1 << CS00))

#  define __TIMER_COUNTER_PRESCALER_256 \
  ((1 << CS02) | (0 << CS01) | (0 << CS00))

#  define __TIMER_COUNTER_PRESCALER_1024 \
  ((1 << CS02) | (0 << CS01) | (1 << CS00))

#endif
