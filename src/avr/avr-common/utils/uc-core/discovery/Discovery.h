/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include "uc-core/fw-configuration/ParticleParameters.h"

#ifdef TRY_INLINE_ISR_RELEVANT
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif

/**
 * increments the port counter
 */
FUNC_ATTRS void dispatchFallingDiscoveryEdge(volatile PulseCounter *portCounter) {
    if (portCounter->counter < RX_DISCOVERY_PULSE_COUNTER_MAX) {
        portCounter->counter++;
    } else {
        portCounter->isConnected = true;
    }
}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif