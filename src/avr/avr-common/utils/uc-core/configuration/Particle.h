/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "uc-core/delay/delay.h"

/**
 * Delay separating particle loops while in discovery state.
 */
#define PARTICLE_DISCOVERY_LOOP_DELAY \
    DELAY_US_30

#define PARTICLE_DISCOVERY_PULSE_DONE_POST_DELAY \
    DELAY_US_500; \
    DELAY_US_150
