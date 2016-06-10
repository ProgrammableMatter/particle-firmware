/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "uc-core/ParticleStateTypes.h"

// DO NOT DECLARE GLOBALS __BEFORE__ PARTICLE ATTRIBUTES!

/**
 * The particle global attributes describe states and contains reception buffers.
 */
volatile ParticleState ParticleAttributes __attribute__ ((section (".noinit")));

// OTHER GLOBALS CAN BE SAFELY DECLARED HERE:

