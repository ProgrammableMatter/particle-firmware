/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include "common/common.h"
#include "ParticleStateTypes.h"
#include "Globals.h"
#include "ParticleCore.h"
#include "uc-core/io-configuration/IoDefinitions.h"
#include "uc-core/delay/delay.h"
#include "uc-core/fw-configuration/ParticleParameters.h"

#  ifdef __AVR_ATtiny1634__

#    include "./delay/delay.h"

#  endif

extern FUNC_ATTRS int particleLoop(void);

/**
 * The particle loop. It changes particle states and performs/execute tasks.
 */
FUNC_ATTRS int particleLoop(void) {
    IO_PORTS_SETUP; // configure input/output pins
#  ifdef __AVR_ATtiny1634__
    DELAY_MS_1; // wait for all nodes to be ready
#  endif
    constructParticleState(&ParticleAttributes);
    ParticleAttributes.node.state = STATE_TYPE_START;
    forever {
        particleTick();
        if (ParticleAttributes.node.state == STATE_TYPE_ERRONEOUS) {
            return 1;
        }
    }
}

