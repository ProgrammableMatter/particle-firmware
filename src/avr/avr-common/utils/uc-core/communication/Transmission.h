/**
 * @author Raoul Rubien
 * 5/2016
 */

#ifndef __TRANSMISSION_INTERRUPTS_H__
#define __TRANSMISSION_INTERRUPTS_H__

#include "Reception.h"

#ifdef SIMULATION

#  include "../../simulation/SimulationMacros.h"

#endif

#  ifdef TRY_INLINE_ISR_RELEVANT
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif