/**
 * @author Raoul Rubien 2016
 *
 * Discovery related arguments.
 */

#pragma once

/**
 * Cut-off value. Pulses counted above are not stored.
 * When number of incoming pulses >= RX_DISCOVERY_PULSE_COUNTER_MAX
 * the affected port is marked as connected.
 */
#define RX_DISCOVERY_PULSE_COUNTER_MAX 30

/**
 * Earliest particle loop when local node discovery may be finished.
 */
#define MIN_NEIGHBOURS_DISCOVERY_LOOPS ((uint8_t)(50))

/**
 * Latest particle loop after local node discovery is to be aborted.
 */
#define MAX_NEIGHBOURS_DISCOVERY_LOOPS ((uint8_t)(250))

/**
 * Latest particle loop when post discovery pulsing to neighbours is to be deactivated.
 */
#define MAX_NEIGHBOUR_PULSING_LOOPS ((uint8_t)(254))

/**
 * Neighbour discovery counter 1 compare A value defines the pulse frequency
 * The lower the value, the higher the frequency.
 */
#ifdef SIMULATION
#define DEFAULT_NEIGHBOUR_SENSING_COUNTER_COMPARE_VALUE ((uint16_t)0x70)
#else
//#define DEFAULT_NEIGHBOUR_SENSING_COUNTER_COMPARE_VALUE ((uint16_t)0x80)
#define DEFAULT_NEIGHBOUR_SENSING_COUNTER_COMPARE_VALUE ((uint16_t)0x90)
#endif

