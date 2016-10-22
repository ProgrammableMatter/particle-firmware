/**
 * @author Raoul Rubien  12.07.2016
 *
 * Local time related arguments.
 */

#pragma once

#include "uc-core/configuration/communication/Communication.h"

/**
 * Put the local time tracking ISR timer counter in phase once the local time is updated by a time package.
 * Disabling macro disables the feature.
 */
#define LOCAL_TIME_IN_PHASE_SHIFTING_ON_LOCAL_TIME_UPDATE

/**
 * In case the phase has to be shifted more than the maximum step
 * the value is capped to the maximum step value.
 */
#define LOCAL_TIME_IN_PHASE_SHIFTING_MAXIMUM_STEP ((uint16_t) 2000)

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
