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

extern FUNC_ATTRS int particleLoop(void);
/**
 * The particle loop. It changes particle states and performs/execute tasks.
 */
FUNC_ATTRS int particleLoop(void) {
    IO_PORTS_SETUP; // configure input/output pins

    constructParticleState(&ParticleAttributes);
    ParticleAttributes.node.state = STATE_TYPE_START;
    forever {
        particleTick();
        if (ParticleAttributes.node.state == STATE_TYPE_ERRONEOUS) {
            return 1;
        }
    }
}

