/**
 * @author Raoul Rubien 2016
 */

#pragma once

/**
 * The maximum numbers of pulses generated for neighbour discovery.
 */
#define RX_DISCOVERY_PULSE_COUNTER_MAX 10

/**
 * Minimum signals to be detected per communication channel until this side is recognized as
 * connected to a neighbour.
 */
#define MIN_RX_NEIGHBOUR_SIGNALS_SENSE ((uint8_t)(10))
/**
 * Earliest particle loop when local node discovery may be finished.
 */
#define MIN_NEIGHBOURS_DISCOVERY_LOOPS ((uint8_t)(50))
/**
 * Latest particle loop when local node discovery is to be aborted.
 */
#define MAX_NEIGHBOURS_DISCOVERY_LOOPS ((uint8_t)(100))
/**
 * Latest particle loop when pulsing to neighbours is to be deactivated.
 */
#define MAX_NEIGHBOUR_PULSING_LOOPS ((uint8_t)(254))

/**
 * Neighbour discovery counter 1 compare A value.
 */
#define DEFAULT_NEIGHBOUR_SENSING_COUNTER_COMPARE_VALUE ((uint16_t)0x80)

/**
 * Initial value for clock delay for Manchester (de-)coding (reception and transmission).
 */
#define DEFAULT_TX_RX_CLOCK_DELAY ((uint16_t) 2000)

/**
 * Maximum short and long time lag. If max. long snapshot lag is exceeded the reception
 * experiences a timeout.
 *
 */
#define DEFAULT_MAX_SHORT_RECEPTION_OVERTIME_PERCENTAGE_RATIO ((uint8_t) 54)
#define DEFAULT_MAX_LONG_RECEPTION_OVERTIME_PERCENTAGE_RATIO ((uint8_t) 104)

/**
 * Heartbeat LED toggles every main-loop count
 */
#define HEARTBEAT_LOOP_COUNT_TOGGLE ((uint8_t)20)
