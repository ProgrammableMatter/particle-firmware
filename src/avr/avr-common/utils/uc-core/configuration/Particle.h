/**
 * @author Raoul Rubien 2016
 */

#pragma once

/**
 * Amount of delay before each particle loop is entered.
 */
#define PARTICLE_LOOP_DELAY
//#define PARTICLE_LOOP_DELAY DELAY_US_5

/**
 * Delay separating particle loops while in discovery state.
 */
#define PARTICLE_DISCOVERY_LOOP_DELAY \
    DELAY_US_15; \
    DELAY_US_15

#define PARTICLE_DISCOVERY_PULSE_DONE_POST_DELAY \
    DELAY_US_150; \
    DELAY_US_150; \
    DELAY_US_150; \
    DELAY_US_150
