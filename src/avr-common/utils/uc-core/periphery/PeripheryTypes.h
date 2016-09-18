/*
 * @author Raoul Rubien 18.09.2016
 *
 * Particle state definition.
 */

#pragma once

#include <stdint.h>

/**
 * Counters/resources needed for non vital platform's periphery such as LEDs, test points and alike.
 */
typedef struct Periphery {
    // particle main loop counter
    uint8_t loopCount;
    uint16_t countdownUntilDeadEndMode;

} volatile Periphery;
