/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related types.
 */

#pragma once

#include <stdint.h>
#include "BasicCalculationTypes.h"

typedef struct LeastSquareRegressionResult {
    /**
    * k as known of f(x)=k*x+d
    */
    CalculationType k;
    /**
    * d as known of f(x)=k*x+d
    */
    CalculationType d;

    /**
    * Indicates whether the fields for calculation results are valid or not.
    */
    uint8_t isCalculationValid : 1;
    uint8_t __pad : 7;

} LeastSquareRegressionResult;