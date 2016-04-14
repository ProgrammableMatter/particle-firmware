/**
 * @author Raoul Rubien 2016
 */

#ifndef PARTICLE_PARAMETERS_H
#define PARTICLE_PARAMETERS_H

/**
 * The maximum numbers of pulses generated for neighbour discovery.
 */
#define RX_DISCOVERY_PULSE_COUNTER_MAX 10

/**
 * Minimum signals to be detected per communication channel until this side is recognized as
 * connected to a neighbour.
 */
#define MIN_RX_NEIGHBOUR_SIGNALS_SENSE ((uint8_t)(6))
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
#define DEFAULT_NEIGHBOUR_SENSING_COUNTER_COMPARE_VALUE ((uint16_t)0x10)

/**
 * Initial value for reception counter compare value. Should be greater than 450 and less than 2032. Note
 * that (2*DEFAULT_TX_RX_COMPARE_TOP_VALUE + 2*ParticleAttributes.rxArguments.receptionDelta) must
 * fit into uint16_t.
 */
#define DEFAULT_TX_RX_COMPARE_TOP_VALUE ((uint16_t)500)

/**
 * Accepted deviation (constant) on reception:
 * Bits received within [DEFAULT_NEIGHBOUR_SENSING_COMPARE_VALUE +/- deviation] are
 * synchronization/rectification bits.
 * Bits received within [DEFAULT_TX_RX_COUNTER_CENTER_VALUE +/- deviation] are data bits.
 */
#define TX_RX_RECEPTION_DELTA ((DEFAULT_TX_RX_COMPARE_TOP_VALUE/4)-1)

/**
 * Initial counter center value and constant acceptance interval range.
 */
#define DEFAULT_TX_RX_COUNTER_CENTER_VALUE (DEFAULT_TX_RX_COMPARE_TOP_VALUE / 2)
#define DEFAULT_TX_RX_COUNTER_LEFT_OF_CENTER_VALUE (DEFAULT_TX_RX_COUNTER_CENTER_VALUE - TX_RX_RECEPTION_DELTA)
#define DEFAULT_TX_RX_COUNTER_RIGHT_OF_CENTER_VALUE (DEFAULT_TX_RX_COUNTER_CENTER_VALUE + TX_RX_RECEPTION_DELTA)

/**
 * Counter top constant acceptance interval range.
 */
#define DEFAULT_TX_RX_COUNTER_LEFT_OF_TOP_VALUE (DEFAULT_TX_RX_COMPARE_TOP_VALUE - TX_RX_RECEPTION_DELTA)
#define DEFAULT_TX_RX_COUNTER_RIGHT_OF_TOP_VALUE (0 + TX_RX_RECEPTION_DELTA)

/**
 * Initial counter compare interrupt value for updating the ongoing reception timeout.
 */
#define DEFAULT_RX_TIMEOUT_INTERRUPT_COMPARE_VALUE (DEFAULT_TX_RX_COUNTER_CENTER_VALUE / 4)

/**
 * Heartbeat LED toggles every main-loop count
 */
#define HEARTBEAT_LOOP_COUNT_TOGGLE ((uint8_t)20)

#endif