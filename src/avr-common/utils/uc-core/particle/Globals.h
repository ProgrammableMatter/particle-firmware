/**
 * @author Raoul Rubien 2016
 *
 * Declaration of the global particle struct.
 * Do not declare globals __BEFORE__ the global Particle struct,
 * since simulated test cases will fail.
 */

#pragma once

#include "uc-core/particle/types/ParticleStateTypes.h"

/**
 * The global particle state structure containing attributes, buffers and alike.
 */
Particle ParticleAttributes __attribute__ ((section (".noinit")));

// Further globals can be safely declared here:
// FooType YourGlobalVariable
