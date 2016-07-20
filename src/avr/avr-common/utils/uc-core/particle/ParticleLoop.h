/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include "ParticleCore.h"

/**
 * The particle loop. It changes particle states and performs/execute tasks.
 */
extern inline void particleLoop(void);

inline void particleLoop(void) {
    IO_PORTS_SETUP; // configure input/output pins
    constructParticle(&ParticleAttributes);
    ParticleAttributes.node.state = STATE_TYPE_START;
    forever {
        particleTick();
    }
}

