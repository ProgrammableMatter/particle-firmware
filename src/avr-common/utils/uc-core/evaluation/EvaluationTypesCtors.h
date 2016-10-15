/*
 * @author Raoul Rubien 15.10.2016
 *
 * Evaluation types constructor implementation.
 */

#pragma once

#include "EvaluationTypes.h"
#include "uc-core/particle/types/NodeAddressTypesCtors.h"

/**
* constructor function
* @param o the object to construct
**/
void constructEvaluation(Evaluation *const o) {
    constructNodeAddress(&o->nextHeatWiresAddress);
}
