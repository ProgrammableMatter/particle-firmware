/*
 * @author Raoul Rubien 15.10.2016
 *
 * Evaluation types constructor implementation.
 */

#pragma once

#include "EvaluationTypes.h"
#include "uc-core/configuration/Evaluation.h"

#ifdef EVALUATION_ENABLE_FLUCTUATE_CPU_FREQUENCY_ON_PURPOSE
/**
 * constructor function
 * @param o the object to construct
 **/
void constructRuntimeOscCalibration(RuntimeOscCalibration *const o) {
    o->initialOscCal = EVALUATION_OSC_CALIBRATION_REGISTER;
    o->step = EVALUATION_OSCCAL_STEP;
    o->minOscCal = o->initialOscCal;
    o->maxOscCal = o->initialOscCal;

    if (o->initialOscCal > EVALUATION_OSCCAL_MIN_OFFSET) {
        o->minOscCal = o->initialOscCal - EVALUATION_OSCCAL_MIN_OFFSET;
    }

    if ((UINT8_MAX - EVALUATION_OSCCAL_MAX_OFFSET) > o->initialOscCal) {
        o->maxOscCal = o->initialOscCal + EVALUATION_OSCCAL_MIN_OFFSET;
    }

    o->isDecrementing = false;
}
#endif

/**
 * constructor function
 * @param o the object to construct
 **/
void constructEvaluation(Evaluation *const o) {
#if defined(EVALUATION_SIMPLE_SYNC_AND_ACTUATION) || defined(EVALUATION_SYNC_CYCLICALLY)
    o->nextHeatWiresAddress.row = 1;
    o->nextHeatWiresAddress.column = 1;
#endif

#ifdef EVALUATION_ENABLE_FLUCTUATE_CPU_FREQUENCY_ON_PURPOSE
    constructRuntimeOscCalibration(&o->oscCalibration);
#endif

#ifdef EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG_THEN_ACTUATE_ONCE
    o->totalSentSyncPackages = 0;
#endif
}
