/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related arguments.
 */

#pragma once

/**
 * The difference of measured synchronization package durations are shifted by the synthetic offset UINT16_MAX/2=0x7fff
 * to overcome the need of signed data types.
 */
#define TIME_SYNCHRONIZATION_SAMPLE_OFFSET ((uint16_t) INT16_MAX)

/**
 * If defined the time approximation will take the time in between the 1st and the last
 * bit of the time package (1st rising, last falling edge). For more information see
 * __SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL_FIRST_RISING_TO_LAST_FALLING_EDGE_630.
 */
#define SYNCHRONIZATION_TIME_PACKAGE_DURATION_COUNTING_FIRST_TO_LAST_BIT_EDGE

/**
 * Mean calculation on-line vs off-line.
 * Uses more flash memory, may speed up the synchronization approximation slightly.
 */
//#define SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION

/**
 * Synchronization strategy.
 */
//#define SYNCHRONIZATION_STRATEGY_RAW_OBSERVATION
#define SYNCHRONIZATION_STRATEGY_MEAN
//#define SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
//#define SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_OUTLIER
//#define SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_MARKED_OUTLIER
//#define SYNCHRONIZATION_ENABLE_ADAPTIVE_MARKED_OUTLIER_REJECTION
//#define SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING

/**
 * Defines the factor f for outlier detection. Samples having values not within
 * [µ - f * σ, µ + f * σ] are rejected.
 */
//#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 3.0)
//#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 2.8)
#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 2.0)
//#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 1.0)

/**
 * Defines the number of manchester clocks in the reference time PDU. Instead measuring the whole package
 * duration (first falling to last rising edge) the the first rising to last falling edge is taken as
 * reference. This has two major reasons:
 * I) The transmitter's manchester coding has not the same delay when generating manchester clock or data
 * signals. Generating clock signals has approx. 6 to 8 instruction more latency versus clock signal. Thus we
 * take the first data signal as package start reference, which is the first riding edge since any package
 * starts with a start bit=1. As pdu end we take the last data bit, which is the last falling edge since the
 * time package has per definition the last bit=0.
 * II) To get slightly more accurate values, one should prefer using falling edges instead of rising, since
 * the flank is steeper.
 *
 * To overcome this complication one may refactor the coding implementation and calculate the manchester code
 * off line. The current design decision (on line coding) was based on saving SRAM.
 */
#define __SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL_FIRST_RISING_TO_LAST_FALLING_EDGE_630 ((float) 63.0)
/**
 * Defines the number of manchester clocks in the reference time PDU.
 * First edge to last edge refers to the whole whole PDU length.
 */
#define __SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL_FIRST_FALLING_TO_LAST_EDGE_640 ((float) 64.0)


#ifdef SYNCHRONIZATION_TIME_PACKAGE_DURATION_COUNTING_FIRST_TO_LAST_BIT_EDGE
#  define SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL \
    __SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL_FIRST_RISING_TO_LAST_FALLING_EDGE_630
#else
#  define SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL \
    __SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL_FIRST_FALLING_TO_LAST_EDGE_640
#endif


#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
//#define __SYNCHRONIZATION_STRATEGY_MEAN_OLD_VALUE_WEIGHT ((float)0.5)
//#define __SYNCHRONIZATION_STRATEGY_MEAN_NEW_VALUE_WEIGHT ((float)0.5)
//#define __SYNCHRONIZATION_STRATEGY_MEAN_OLD_VALUE_WEIGHT ((float)0.9)
//#define __SYNCHRONIZATION_STRATEGY_MEAN_NEW_VALUE_WEIGHT ((float)0.1)
#define __SYNCHRONIZATION_STRATEGY_MEAN_OLD_VALUE_WEIGHT ((float)0.75)
#define __SYNCHRONIZATION_STRATEGY_MEAN_NEW_VALUE_WEIGHT ((float)0.25)
//#define __SYNCHRONIZATION_STRATEGY_MEAN_OLD_VALUE_WEIGHT ((float)0.99)
//#define __SYNCHRONIZATION_STRATEGY_MEAN_NEW_VALUE_WEIGHT ((float)0.01)
#define SYNCHRONIZATION_STRATEGY_MEAN_OLD_VALUE_WEIGHT __SYNCHRONIZATION_STRATEGY_MEAN_OLD_VALUE_WEIGHT
#define SYNCHRONIZATION_STRATEGY_MEAN_NEW_VALUE_WEIGHT __SYNCHRONIZATION_STRATEGY_MEAN_NEW_VALUE_WEIGHT
#endif
