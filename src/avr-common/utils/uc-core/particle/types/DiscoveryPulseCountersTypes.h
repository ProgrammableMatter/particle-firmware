/*
 * @author Raoul Rubien 09.10.2016
 *
 * Discovery state definition.
 */

#pragma once

#include <stdint.h>
#include "uc-core/discovery/DiscoveryTypes.h"

/**
 * Stores the amount of incoming pulses for each communication channel. The isConnected flags are set
 * if the number of incoming pulses exceeds a specific threshold.
 */
typedef struct DiscoveryPulseCounters {
    DiscoveryPulseCounter north;
    DiscoveryPulseCounter east;
    DiscoveryPulseCounter south;
    // discovery loop counter
    uint8_t loopCount;
} DiscoveryPulseCounters;