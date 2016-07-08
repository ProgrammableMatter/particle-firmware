/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include "common/common.h"
#include "ParticleStateTypes.h"
#include "Globals.h"
#include "ParticleCore.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/delay/delay.h"
#include "uc-core/configuration/Particle.h"

extern inline int particleLoop(void);
/**
 * The particle loop. It changes particle states and performs/execute tasks.
 */
inline int particleLoop(void) {
    IO_PORTS_SETUP; // configure input/output pins
    constructParticle(&ParticleAttributes);
    ParticleAttributes.node.state = STATE_TYPE_START;
    forever {
        PARTICLE_MAIN_LOOP_DELAY;
        particleTick();
//        if (ParticleAttributes.node.state == STATE_TYPE_ERRONEOUS) {
//            return 1;
//        }
    }
}

