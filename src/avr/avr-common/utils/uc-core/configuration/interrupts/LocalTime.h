/**
 * @author Raoul Rubien
 * 20.07.2016
 */

#pragma once

#include "TimerCounter1.h"

/**
 *  define timer / counter 0/1 reception/transmission interrupt macros
 */
#if  defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)

#  define LOCAL_TIME_INTERRUPT_COMPARE_VALUE (OCR1B)

#  define LOCAL_TIME_INTERRUPT_COMPARE_ENABLE \
    __TIMER1_INTERRUPT_CLEAR_PENDING_COMPARE_B; \
    __TIMER1_COMPARE_B_INTERRUPT_ENABLE

#  define LOCAL_TIME_INTERRUPT_COMPARE_DISABLE \
    __TIMER1_COMPARE_B_INTERRUPT_DISABLE

#  else
#    error
#  endif
