/**
 * @author Raoul Rubien  12.07.2016
 *
 * Local time related arguments.
 */

#pragma once

#include "uc-core/configuration/communication/Communication.h"

///**
// * Defines at which percentage of UINT16_MAX the local time tracking timer/counter ISR fires initially.
// * The value is approximated at runtime with each received time synchronization package.
// */
////#define LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_75_PERCENT
//#define LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_80_PERCENT
////#define LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_85_PERCENT
////#define LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_90_PERCENT
//
//#define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_75_PERCENT_OF_UINT16_MAX ((uint16_t) 49151)
//#define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_80_PERCENT_OF_UINT16_MAX ((uint16_t) 52428)
//#define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_85_PERCENT_OF_UINT16_MAX ((uint16_t) 55704)
//#define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_90_PERCENT_OF_UINT16_MAX ((uint16_t) 58982)
//
//#if defined(LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_75_PERCENT)
//#  define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_80_PERCENT_OF_UINT16_MAX
//#  define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE (0.75)
//#else
//#  if defined(LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_80_PERCENT)
//#    define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_80_PERCENT_OF_UINT16_MAX
//#    define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE (0.8)
//#  else
//#    if defined(LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_85_PERCENT)
//#      define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_80_PERCENT_OF_UINT16_MAX
//#      define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE (0.85)
//#    else
//#      if defined(LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_90_PERCENT)
//#        define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_80_PERCENT_OF_UINT16_MAX
//#        define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE (0.9)
//#      endif
//#    endif
//#  endif
//#endif

///**
// * The default compare match start value for the local time tracking timer/counter ISR.
// */
//#define LOCAL_TIME_DEFAULT_INTERRUPT_DELAY __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY

///**
// * The working point as (LOCAL_TIME_DEFAULT_INTERRUPT_DELAY / UINT16_MAX).
// */
//#define LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE

///**
// * The rather simple experimental shows an example how a clock shift could be approached.
// * Never the less there is too less flash memory left on device to implement a better, non naive approach.
// * Enable the define to enable the source.
// */
//#define LOCAL_TIME_EXPERIMENTAL_IN_PHASE_APPROXIMATION_SHIFT

/**
 * Defines the working point of the local time tracking interrupt.
 * I.e. if the incoming manchester clock is observed to be 1021, the
 * local time tracking will trigger each 51*1021=52071 clocks.
 */
#define LOCAL_TIME_TRACKING_INT_DELAY_MANCHESTER_CLOCK_MULTIPLIER ((uint8_t) 51)

/**
 * The initial local time tracking ISR separation in clocks.
 */
#define LOCAL_TIME_TRACKING_INT_DELAY_MANCHESTER_CLOCK_INITIAL_VALUE \
    ((uint16_t) LOCAL_TIME_TRACKING_INT_DELAY_MANCHESTER_CLOCK_MULTIPLIER * \
    COMMUNICATION_DEFAULT_TX_RX_CLOCK_DELAY)
