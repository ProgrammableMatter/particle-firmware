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
 * Clock speed vs. minimum error ratio table, deprecated transmitter isr handling:
 *
 * clock delay | short interval             | long interval
 *             | opt speed                  | opt speed
 *             |                opt size    |               opt size
 * --------------------------------------------------------------------
 * 180           66             86            121           150
 * 256           63             75            115           121
 * 512           56             63            107           108
 * 1024          53             57            104           104
 * 2048          52             54            102           102
 */
/**
 * Clock speed vs. minimum error ratio table (transmitter handles isr synchronous)
 *
 * clock delay | short interval             | long interval             | is setting
 *             | opt speed                  | opt speed                 | usable
 *             |                opt size    |               opt size    |
 * ---------------------------------------------------------------------------------
 * 180           NA              NA            NA            NA           false
 * 256           xxx             68            xxx           105          false
 * 512           xxx             59            xxx           102          false
 * 1024          xxx             55            xxx           101          true
 * 2048          xxx             53            xxx           101          true
 */
/**
 * Initial value for clock delay for Manchester (de-)coding (reception and transmission).
 */
#define DEFAULT_TX_RX_CLOCK_DELAY ((uint16_t) 1024)

/**
 * Maximum short and long time lag. If max. long snapshot lag is exceeded the reception
 * experiences a timeout.
 *
 */
#define DEFAULT_MAX_SHORT_RECEPTION_OVERTIME_PERCENTAGE_RATIO ((uint8_t) 59)
#define DEFAULT_MAX_LONG_RECEPTION_OVERTIME_PERCENTAGE_RATIO ((uint8_t) 102)

/**
 * Heartbeat LED toggles every main-loop count
 */
#define HEARTBEAT_LOOP_COUNT_TOGGLE ((uint8_t)20)
