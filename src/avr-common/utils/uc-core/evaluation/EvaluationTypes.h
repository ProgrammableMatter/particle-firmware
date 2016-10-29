/*
 * @author Raoul Rubien 15.10.2016
 *
 * Evaluation state definition.
 */

#pragma once

#include <stdint.h>
#include "uc-core/particle/types/NodeAddressTypes.h"
#include "uc-core/configuration/Evaluation.h"

#ifdef EVALUATION_ENABLE_FLUCTUATE_CPU_FREQUENCY_ON_PURPOSE
typedef struct RuntimeOscCalibration {
    uint8_t initialOscCal;
    uint8_t step;
    uint8_t minOscCal;
    uint8_t maxOscCal;
    uint8_t isDecrementing : 1;
    uint8_t __pad : 7;

} RuntimeOscCalibration;
#endif

typedef struct Evaluation {
#if defined(EVALUATION_SIMPLE_SYNC_AND_ACTUATION) || defined(EVALUATION_SYNC_CYCLICALLY)
    /**
     * network address to issue next heat wires command at
     **/
    NodeAddress nextHeatWiresAddress;
#endif

#ifdef EVALUATION_ENABLE_FLUCTUATE_CPU_FREQUENCY_ON_PURPOSE
    /**
     * OSCCAL runtime boundaries
     */
    RuntimeOscCalibration oscCalibration;
#endif

#ifdef EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG_THEN_ACTUATE_ONCE
    /**
     * total number of currently sent sync packages
     */
    uint16_t totalSentSyncPackages;
#endif
} Evaluation;
