/*
 * @author Raoul Rubien 15.10.2016
 *
 * Evaluation types constructor implementation.
 */

#pragma once

#include "EvaluationTypes.h"

/**
* constructor function
* @param o the object to construct
**/
void constructEvaluation(Evaluation *const o) {
    o->nextHeatWiresAddress.row = 1;
    o->nextHeatWiresAddress.column = 1;
}
