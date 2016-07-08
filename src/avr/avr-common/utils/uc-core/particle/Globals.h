/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "ParticleStateTypes.h"

// DO NOT DECLARE GLOBALS __BEFORE__ PARTICLE ATTRIBUTES!

/**
 * The global particle state structure containing attributes, buffers and alike.
 */
volatile Particle ParticleAttributes __attribute__ ((section (".noinit")));

// OTHER GLOBALS CAN BE SAFELY DECLARED HERE:

