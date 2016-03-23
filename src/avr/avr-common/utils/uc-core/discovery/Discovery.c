/**
 * @author Raoul Rubien 2015
 */


/**
 * increments the port counter
 */
static inline void dispatchFallingDiscoveryEdge(volatile PulseCounter *portCounter) {
    if (portCounter->counter < RX_DISCOVERY_PULSE_COUNTER_MAX) {
        portCounter->counter++;
    }
}

