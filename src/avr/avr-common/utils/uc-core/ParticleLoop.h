/**
 * @author Raoul Rubien 2015
 */
#ifndef __PARTICLE_MAIN_LOOP_H
#define __PARTICLE_MAIN_LOOP_H

/**
* @author Raoul Rubien 2015
*/
#include "ParticleLoop.h"

#include <avr/interrupt.h>
#include <common/common.h>
#include "IoDefinitions.h"
#include "interrupts/InterruptDefinitions.jail"
//#include "Interrupts.c"
#include "ParticleTypes.h"
#include "Globals.h"
#include "Particle.h"

#ifdef __AVR_ATtiny1634__

#  include <util/delay.h>

#endif

static inline int particleLoop(void);

/**
 * The particle loop. It changes particle states and performs/execute tasks.
 */
static inline int particleLoop(void) {
    IO_PORTS_SETUP; // configure input/output pins
#ifdef __AVR_ATtiny1634__
    _delay_ms(1); // wait for all nodes to be ready
#endif

    ParticleAttributes.node.state = STATE_TYPE_START;
    forever {
        particleTick();
        if (ParticleAttributes.node.state == STATE_TYPE_ERRONEOUS) {
            return 1;
        }
    }
}


#endif
