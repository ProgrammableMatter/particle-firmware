/**
 * @author Raoul Rubien 2015
 */
#ifndef __DISCOVERY_H
#define __DISCOVERY_H

/**
 * increments the port counter
 */
inline void dispatchFallingDiscoveryEdge(volatile PulseCounter *portCounter) {
    if (portCounter->counter < RX_DISCOVERY_PULSE_COUNTER_MAX) {
        portCounter->counter++;
    }
}

#endif