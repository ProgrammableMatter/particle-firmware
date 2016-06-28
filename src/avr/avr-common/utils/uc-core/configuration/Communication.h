/**
 * @author Raoul Rubien 2016
 */

#pragma once



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
 * --- 1170 is the max clock length due to counter adjustment impl. limitation ---
 * 2048          xxx             53            xxx           101          true
 *
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

/**
 * Number of buffer bytes for reception and transmission. Received snapshots are decoded to
 * the reception buffer. Data to be sent is read from the transmission buffer.
 */
#define TX_RX_NUMBER_BUFFER_BYTES 7

/**
 * Size of reception snapshot buffer per port. Sould be >= TX_RX_NUMBER_BUFFER_BYTES * 8 * 2 + 2
 */
#define RX_NUMBER_SNAPSHOTS 114


