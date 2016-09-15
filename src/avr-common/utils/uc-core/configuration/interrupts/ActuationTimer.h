/**
 * @author Raoul Rubien 20.07.2016
 *
 * Actuator interrupt related configuration.
 */

#pragma once

#include "TimerCounter0.h"
#include "uc-core/configuration/Actuation.h"

/**
 *  actuation interrupt macros
 */
#if  defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)

# define __ACTUATOR_COMPARE_VALUE_SETUP(compareValue) \
    __TIMER0_INTERRUPT_COMPARE_VALUE_SETUP(compareValue)

# define __ACTUATOR_TIMER_VALUE_SETUP(compareValue) \
    (__TIMER0_INTERRUPT_TIMER_VALUE_SETUP(compareValue))

#  define __ACTUATOR_COUNTER_PRESCALER_VALUE \
    __TIMER_COUNTER_PRESCALER_64

#  define __ACTUATOR_COUNTER_PRESCALER_ENABLE \
    __TIMER0_INTERRUPT_PRESCALER_ENABLE(__ACTUATOR_COUNTER_PRESCALER_VALUE)

#  define __ACTUATOR_COUNTER_PRESCALER_DISABLE \
    __TIMER0_INTERRUPT_PRESCALER_DISABLE

#  define ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_STRONG \
    __ACTUATOR_COMPARE_VALUE_SETUP(ACTUATION_COMPARE_VALUE_POWER_STRONG)

#  define ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_MEDIUM \
    __ACTUATOR_COMPARE_VALUE_SETUP(ACTUATION_COMPARE_VALUE_POWER_MEDIUM)

#  define ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_WEAK \
    __ACTUATOR_COMPARE_VALUE_SETUP(ACTUATION_COMPARE_VALUE_POWER_WEAK)

#  define ACTUATOR_INTERRUPT_ENABLE \
    __ACTUATOR_TIMER_VALUE_SETUP(UINT8_MAX); \
    __ACTUATOR_COUNTER_PRESCALER_ENABLE

#  define ACTUATOR_INTERRUPT_DISABLE \
    __ACTUATOR_COUNTER_PRESCALER_DISABLE

# define ACTUATOR_INTERRUPT_SETUP \
    ACTUATOR_INTERRUPT_DISABLE; \
    __TIMER0_OVERFLOW_INTERRUPT_DISABLE; \
    __TIMER0_INTERRUPT_OUTPUT_MODE_DISCONNECTED_SETUP; \
    __TIMER0_INTERRUPT_WAVE_GENERATION_MODE_PWM_PHASE_CORRECT_SETUP; \
    __ACTUATOR_TIMER_VALUE_SETUP(0); \
    __ACTUATOR_COMPARE_VALUE_SETUP(ACTUATION_COMPARE_VALUE_POWER_MEDIUM); \
    __TIMER0_COMPARE_INTERRUPT_ENABLE

#  else
#    error
#  endif
