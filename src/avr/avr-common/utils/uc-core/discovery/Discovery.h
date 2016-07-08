/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include "uc-core/configuration/Discovery.h"


extern FUNC_ATTRS void dispatchFallingDiscoveryEdge(volatile DiscoveryPulseCounter *portCounter);
/**
 * increments the port counter
 */
FUNC_ATTRS void dispatchFallingDiscoveryEdge(volatile DiscoveryPulseCounter *portCounter) {
    if (portCounter->counter < RX_DISCOVERY_PULSE_COUNTER_MAX) {
        portCounter->counter++;
    } else {
        portCounter->isConnected = true;
    }
}