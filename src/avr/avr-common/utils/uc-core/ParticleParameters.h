/**
 * @author Raoul Rubien 2016
 */

#ifndef PARTICLE_PARAMETERS_H
#define PARTICLE_PARAMETERS_H

/**
 * The maximum numbers of pulses generated for neighbour discovery.
 */
#define RX_DISCOVERY_PULSE_COUNTER_MAX 0xA

/**
 * Minimum signals to be detected per communication channel until this side is recognized as
 * connected to a neighbour.
 */
#define MIN_RX_NEIGHBOUR_SIGNALS_SENSE ((unsigned char)(RX_DISCOVERY_PULSE_COUNTER_MAX / 2.0)  + 1)
/**
 * Earliest particle loop when local node discovery may be finished.
 */
#define MIN_NEIGHBOURS_DISCOVERY_LOOPS (unsigned char)(5 * RX_DISCOVERY_PULSE_COUNTER_MAX)
/**
 * Latest particle loop when local node discovery is to be aborted.
 */
#define MAX_NEIGHBOURS_DISCOVERY_LOOPS (unsigned char)(2 * MIN_NEIGHBOURS_DISCOVERY_LOOPS)
/**
 * Latest particle loop when pulsing to neighbours is to be deactivated.
 */
#define MAX_NEIGHBOUR_PULSING_LOOPS (unsigned char)(3 * MIN_NEIGHBOURS_DISCOVERY_LOOPS)

/**
 * Neighbour discovery counter 1 compare A value.
 */
#define DEFAULT_NEIGHBOUR_SENSING_COMPARE_VALUE 0x10

/**
 * Reception counter 1 compare A value. Must be less than 2032
 */
#define DEFAULT_TX_RX_COMPARE_TOP_VALUE 500

/**
 * Heartbeat LED toggles every main-loop count
 */
#define HEARTBEAT_LOOP_COUNT_TOGGLE 20

#endif