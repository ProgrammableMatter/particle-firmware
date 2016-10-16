/**
 * @author Raoul Rubien  12.07.2016
 *
 * Local time related arguments.
 */

#pragma once

#include "uc-core/configuration/communication/Communication.h"

/**
 * The rather simple experimental shows an example how a clock shift could be approached.
 * Never the less there is too less flash memory left on device to implement a better, non naive approach.
 * Enable the define to enable the source.
 */
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
