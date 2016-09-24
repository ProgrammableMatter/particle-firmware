/**
 * @author Raoul Rubien 2016
 *
 * The main loop implementation.
 */

#pragma once

#include "ParticleCore.h"

/**
 * The main particle loop. It repetitively calls the state driven particle core implementation.
 */
extern inline void processLoop(void) __attribute__ ((noreturn));

inline void processLoop(void) {
    IO_PORTS_SETUP; // configure input/output pins
    constructParticle(&ParticleAttributes);
    ParticleAttributes.node.state = STATE_TYPE_START;

    forever {
        process();
    }
}

