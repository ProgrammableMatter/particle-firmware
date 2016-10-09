/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related types.
 */

#pragma once

#include "LeastSquareRegressionTypes.h"

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructLeastSquareRegressionResult(LeastSquareRegressionResult *const o) {
    o->k = 0;
    o->d = 0;
}