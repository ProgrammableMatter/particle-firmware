/*
 * @author Raoul Rubien 09.10.2016
 *
 * Discovery types constructor implementation.
 */

#pragma once

#include "DiscoveryTypes.h"

/**
 * constructor function
 * @param o the object to construct
 */
void constructDiscoveryPulseCounter(DiscoveryPulseCounter *const o) {
    o->counter = 0;
    o->isConnected = false;
}