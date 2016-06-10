/**
 * @author Raoul Rubien
 * 5/2016
 */

#pragma once

#include "Reception.h"

#ifdef SIMULATION

#  include "simulation/SimulationMacros.h"

#endif


#ifdef TRY_INLINE_ISR_RELEVANT
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif


#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif