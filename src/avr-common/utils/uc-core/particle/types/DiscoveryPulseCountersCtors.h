/*
 * @author Raoul Rubien 09.10.2016
 *
 * Discovery types constructor implementation.
 */

#pragma once

#include "DiscoveryPulseCountersTypes.h"
#include "uc-core/discovery/DiscoveryTypesCtors.h"

/**
 * constructor function
 * @param o the object to construct
 */
void constructDiscoveryPulseCounters(DiscoveryPulseCounters *const o) {
    constructDiscoveryPulseCounter(&o->north);
    constructDiscoveryPulseCounter(&o->east);
    constructDiscoveryPulseCounter(&o->south);
    o->loopCount = 0;
}
