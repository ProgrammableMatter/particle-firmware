/**
 * @author Raoul Rubien 2016
 */

#ifndef __PARTICLE_GLOBALS_H
#define __PARTICLE_GLOBALS_H

#include "./ParticleTypes.h"

// DO NOT DECLARE GLOBALS BEFORE PARTICLE ATTRIBUTES!

/**
 * The particle global attributes describe states and contains reception buffers.
 */
volatile ParticleState ParticleAttributes __attribute__ ((section (".noinit")));

// OTHER GLOBALS CAN BE SAFELY DECLARED HERE:

#endif
