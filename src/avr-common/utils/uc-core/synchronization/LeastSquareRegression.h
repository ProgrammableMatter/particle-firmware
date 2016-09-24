/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related implementation.
 */

#pragma once

#include "SynchronizationTypes.h"

typedef uint16_t MeasurementValueType;
typedef float CalculationType;
typedef float DeviationType;

typedef struct {
    MeasurementValueType xValues[10];
    MeasurementValueType durations[10];
    uint8_t numValues;
    float k;
    float d;
    float variance;
} Measurement;

Measurement measurements =//__attribute__ ((section (".noinit"))) =
        {
                .xValues  = {1, 2, 3, 4},
                .durations = {6, 5, 7, 10},
                .numValues = 4,
//                .k = 0,
//                .d = 0,
                .variance = 0,
        };

//Measurement measurements =
//        {
//                .xValues  = {1, 2, 3, 4, 5},
//                .durations = {1, 1, 1, 1, 1},
//                .numValues =5,
//        };

typedef uint16_t CumulationType;



/**
 * calculating Linear least squares fitting function for measured values
 * https://en.wikipedia.org/wiki/Linear_least_squares_(mathematics)#Orthogonal_decomposition_methods
 */
void calculateFittingFunction(MeasurementFiFoBuffer *measurements) {
    CumulationType a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
    CumulationType x = 0, y = 0;
    DeviationType mean = 0;

    for (uint8_t idx = 0; idx < measurements.numValues; idx++) {
        y = measurements.durations[idx];

        // TODO optimize: pull 2-factor out of loop
        a += 2 * idx * idx;
        b += 2 * idx * y;
        c += 2 * idx;

        d += 2;
        e += 2 * y;
        //f += 2 * idx;

        mean += y;
    }
    f = c;

    mean /= measurements.numValues;

    for (uint8_t idx = 0; idx < measurements.numValues; idx++) {
        y = measurements.durations[idx];
        CalculationType diff;
        if (y >= mean) {
            diff = y - mean;
        } else {
            diff = mean - y;
        }
        measurements.variance += diff * diff;
    }
    measurements.variance /= measurements.numValues;
    measurements.k = (d * b - c * e) / (CalculationType) (d * a - f * c);
    measurements.d = (-measurements.k * a + b) / c;
}