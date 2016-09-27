/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related types.
 */

#pragma once

#include <stdint.h>
#include "uc-core/configuration/Synchronization.h"
#include "LeastSquareRegressionTypes.h"
#include "SampleFifoTypes.h"

#ifdef C_STRUCTS_TO_JSON_PARSER_TYPEDEF_NOT_SUPPORTED_SUPPRESS_REGULAR_TYPEDEFS
# define CumulationType uint32_t
# define SampleValueType uint16_t
# define CalculationType float
#define IndexType uint8_t
#else
typedef uint32_t CumulationType;
typedef uint16_t SampleValueType;
typedef float CalculationType;
typedef uint8_t IndexType;
#endif

typedef struct TimeSynchronization {
    SamplesFifoBuffer timeIntervalSamples;
    LeastSquareRegressionResult fittingFunction;
    /**
    * samples' mean
    */
    CalculationType mean;
    /**
     * The progressive mean has kind of knowledge of previous values and is the 1/2 of the last progressive mean and the current value.
     */
    CumulationType progressiveMean;
    /**
    * samples' variance
    */
    CalculationType variance;
    /**
     * samples' standard deviation
     */
    CalculationType stdDeviance;
    /**
     * next local time stamp when transmisson is to be initialized
     */
    uint16_t nextSyncPackageTransmissionStartTime;
    /**
     * Indicates whether the fields for calculation results are valid or not.
     */
    uint8_t isCalculationValid : 1;
    /**
     * if flag is set, transmission of further synchronization package is scheduled
     */
    volatile uint8_t isNextSyncPackageTransmissionEnabled : 1;
    uint8_t __pad : 6;

} TimeSynchronization;