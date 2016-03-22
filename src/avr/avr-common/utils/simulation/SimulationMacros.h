/**
 * @author Raoul Rubien 2016
 */

#ifndef SIMULATION_MACROS_H
#define SIMULATION_MACROS_H


#if defined(__AVR_ATmega16__)
#include "../uc-core/Globals.h"
extern volatile ParticleState ParticleAttributes;
#  define IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE ParticleAttributes.state = STATE_TYPE_ERRONEOUS
#  define IS_SIMULATION 1
#else
#  define IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE
#endif

#endif

