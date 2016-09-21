/**
 * @author Raoul Rubien 2016
 *
 * Particle related arguments.
 */

#pragma once

/**
 * Delay separating particle loops while in discovery state.
 */
#define PARTICLE_DISCOVERY_LOOP_DELAY \
    DELAY_US_30

/**
 * Delay separating the following state switch after discovery has finished.
 */
#define PARTICLE_DISCOVERY_PULSE_DONE_POST_DELAY \
    DELAY_MS_1; \
    DELAY_MS_1; \
    DELAY_MS_1; \
    DELAY_US_500
