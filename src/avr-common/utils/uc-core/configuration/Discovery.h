/**
 * @author Raoul Rubien 2016
 *
 * Discovery related arguments.
 */

#pragma once

/**
 * Cut-off value. Pulses counted above are not stored.
 */
#define RX_DISCOVERY_PULSE_COUNTER_MAX 10

/**
 * Earliest particle loop when local node discovery may be finished.
 */
#define MIN_NEIGHBOURS_DISCOVERY_LOOPS ((uint8_t)(50))

/**
 * Latest particle loop after local node discovery is to be aborted.
 */
#define MAX_NEIGHBOURS_DISCOVERY_LOOPS ((uint8_t)(100))

/**
 * Latest particle loop when post discovery pulsing to neighbours is to be deactivated.
 */
#define MAX_NEIGHBOUR_PULSING_LOOPS ((uint8_t)(254))

/**
 * Neighbour discovery counter 1 compare A value defines the pulse frequencs.
 * The lower the value, the higher the frequency.
 */
#define DEFAULT_NEIGHBOUR_SENSING_COUNTER_COMPARE_VALUE ((uint16_t)0x80)
