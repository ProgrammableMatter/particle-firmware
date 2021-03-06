/*
 * @author Raoul Rubien 09.10.2016
 *
 * Discovery state definition.
 */

#pragma once

#include <stdint.h>

/**
 * The discovery pulse counter stores the amount of discovery pulses and the connectivity state.
 */
typedef struct DiscoveryPulseCounter {
    // discovery pulse counter
    volatile uint8_t counter : 5;
    // connectivity flag
    volatile uint8_t isConnected : 1;
    volatile uint8_t __pad : 2;
} DiscoveryPulseCounter;
