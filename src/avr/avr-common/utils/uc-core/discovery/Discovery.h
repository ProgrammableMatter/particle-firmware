/**
 * @author Raoul Rubien 2015
 */
#ifndef __DISCOVERY_H
#define __DISCOVERY_H

#  define FUNC_ATTRS inline
/**
 * increments the port counter
 */
FUNC_ATTRS void dispatchFallingDiscoveryEdge(volatile PulseCounter *portCounter) {
    if (portCounter->counter < RX_DISCOVERY_PULSE_COUNTER_MAX) {
        portCounter->counter++;
    }
}

# ifdef FUNC_ATTRS
#   undef FUNC_ATTRS
#  endif
#endif