/**
 * @author Raoul Rubien 2015
 */
#ifndef __PARTICLE_MAIN_LOOP_H
#define __PARTICLE_MAIN_LOOP_H
#include <common/common.h>
#include "IoDefinitions.h"
#include "ParticleTypes.h"
#include "Globals.h"
#include "Particle.h"
#include "./delay/delay.h"
#include "./ParticleParameters.h"

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

#  ifdef __AVR_ATtiny1634__
#    include "./delay/delay.h"
#  endif

FUNC_ATTRS int particleLoop(void);

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

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif
