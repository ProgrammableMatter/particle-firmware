/**
 * @author Raoul Rubien  16.10.2016
 *
 * Evaluation related arguments.
 */

#pragma once

//#define EVALUATION_SIMPLE_SYNC_AND_ACTUATION
//#define EVALUATION_SYNC_CYCLICALLY
//#define EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG
//#define EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG_IN_PHASE_SHIFTING
#define EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG_THEN_ACTUATE_ONCE


/**
 * increment / decrement the cpu frequency after several transmitted sync packages
 * enable to enable oscillating frequency
 * disable for constant frequency
 */
//#define EVALUATION_ENABLE_FLUCTUATE_CPU_FREQUENCY_ON_PURPOSE

#ifdef EVALUATION_ENABLE_FLUCTUATE_CPU_FREQUENCY_ON_PURPOSE
#  if defined(__AVR_ATmega16__)
#    define EVALUATION_OSC_CALIBRATION_REGISTER OSCCAL
#  endif

#  if defined(__AVR_ATtiny1634__)
#    define EVALUATION_OSC_CALIBRATION_REGISTER OSCCAL0
#  endif

#  define EVALUATION_OSCCAL_MAX_OFFSET ((uint8_t)8)
#  define EVALUATION_OSCCAL_MIN_OFFSET ((uint8_t)8)
#  define EVALUATION_OSCCAL_STEP ((uint8_t)1)
#endif


#ifdef EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG_THEN_ACTUATE_ONCE
#define EVALUATION_SYNC_PACKAGES_BEFORE_ACTUATION ((uint16_t) 20)
#endif