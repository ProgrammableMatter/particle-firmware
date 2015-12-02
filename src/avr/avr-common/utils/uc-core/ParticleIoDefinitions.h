/**
 * @author Raoul Rubien 2015
 */

#ifndef PROJECT_PARTICLEPORTDEFINICIONS_H
#define PROJECT_PARTICLEPORTDEFINICIONS_H

#include <common/PortADefinition.h>
#include <common/PortBDefinition.h>
#ifdef __AVR_ATmega16__
#  include <common/PortDDefinition.h>
#endif

#include <common/PortInteraction.h>

// tx south
#define SOUTH_TX_PIN Pin4
#define SOUTH_TX_DIR ADir
#define SOUTH_TX_OUT AOut
#define SOUTH_TX_IN AOut

//#define SOUTH_TX_SETUP SOUTH_TX_DIR setOut SOUTH_TX_PIN
#define SOUTH_TX_HI SOUTH_TX_OUT setHi SOUTH_TX_PIN
#define SOUTH_TX_LO SOUTH_TX_OUT setLo SOUTH_TX_PIN
#define SOUTH_TX_SEND_HI SOUTH_TX_LO
#define SOUTH_TX_SEND_LO SOUTH_TX_HI
#define SOUTH_TX_IS_HI (0 != (SOUTH_TX_IN getBit SOUTH_TX_PIN))
#define SOUTH_TX_IS_LO (0 == (SOUTH_TX_IN getBit SOUTH_TX_PIN))
#define SOUTH_TX_TOGGLE SOUTH_TX_OUT toggleBit SOUTH_TX_PIN

// rx south
#ifdef __AVR_ATtiny20__
#  define SOUTH_RX_PIN Pin5
#  define SOUTH_RX_DIR ADir
#  define SOUTH_RX_OUT AOut
#  define SOUTH_RX_IN AIn
#elif __AVR_ATmega16__
#  define SOUTH_RX_PIN Pin3
#  define SOUTH_RX_DIR DDir
#  define SOUTH_RX_OUT DOut
#  define SOUTH_RX_IN DIn
#else
#  error
#endif

#define SOUTH_RX_PULL_UP SOUTH_RX_OUT setHi SOUTH_RX_PIN
#define SOUTH_RX_PULL_DOWN SOUTH_RX_OUT setLo SOUTH_RX_PIN
#define SOUTH_RX_IS_HI (0 != (SOUTH_RX_IN getBit SOUTH_RX_PIN))
#define SOUTH_RX_IS_LO (0 == (SOUTH_RX_IN getBit SOUTH_RX_PIN))

// rx-switch south
#define SOUTH_RX_SWITCH_PIN Pin6
#define SOUTH_RX_SWITCH_DIR ADir
#define SOUTH_RX_SWITCH_OUT AOut
#define SOUTH_RX_SWITCH_IN AOut

//#define SOUTH_RX_SWITCH_SETUP SOUTH_RX_SWITCH_DIR setOut SOUTH_RX_SWITCH_PIN
#define SOUTH_RX_SWITCH_HI SOUTH_RX_SWITCH_OUT setHi SOUTH_RX_SWITCH_PIN
#define SOUTH_RX_SWITCH_LO SOUTH_RX_SWITCH_OUT setLo SOUTH_RX_SWITCH_PIN
#define SOUTH_RX_SWITCH_IS_HI (0 != (SOUTH_RX_SWITCH_IN getBit SOUTH_RX_SWITCH_PIN))
#define SOUTH_RX_SWITCH_IS_LO (0 == (SOUTH_RX_SWITCH_IN getBit SOUTH_RX_SWITCH_PIN))

// tx north
#define NORTH_TX_PIN Pin1
#define NORTH_TX_DIR ADir
#define NORTH_TX_OUT AOut
#define NORTH_TX_IN AOut

//#define NORTH_TX_SETUP NORTH_TX_DIR setOut NORTH_TX_PIN
#define NORTH_TX_HI NORTH_TX_OUT setHi NORTH_TX_PIN
#define NORTH_TX_LO NORTH_TX_OUT setLo NORTH_TX_PIN
#define NORTH_TX_SEND_HI NORTH_TX_LO
#define NORTH_TX_SEND_LO NORTH_TX_HI
#define NORTH_TX_IS_HI (0 != (NORTH_TX_IN getBit NORTH_TX_PIN))
#define NORTH_TX_IS_LO (0 == (NORTH_TX_IN getBit NORTH_TX_PIN))
#define NORTH_TX_TOGGLE NORTH_TX_OUT toggleBit NORTH_TX_PIN

// rx north
#ifdef __AVR_ATtiny20__
#  define NORTH_RX_PIN Pin2
#  define NORTH_RX_DIR ADir
#  define NORTH_RX_OUT AOut
#  define NORTH_RX_IN AIn
#elif __AVR_ATmega16__
#  define NORTH_RX_PIN Pin2
#  define NORTH_RX_DIR DDir
#  define NORTH_RX_OUT DOut
#  define NORTH_RX_IN DIn
#else
#  error
#endif

#define NORTH_RX_HI NORTH_RX_OUT setHi NORTH_RX_PIN
#define NORTH_RX_LO NORTH_RX_OUT setLo NORTH_RX_PIN
#define NORTH_RX_IS_HI (0 != (NORTH_RX_IN getBit NORTH_RX_PIN))
#define NORTH_RX_IS_LO (0 == (NORTH_RX_IN getBit NORTH_RX_PIN))

// rx-switch north
#define NORTH_RX_SWITCH_PIN Pin0
#define NORTH_RX_SWITCH_DIR ADir
#define NORTH_RX_SWITCH_OUT AOut
#define NORTH_RX_SWITCH_IN AOut

//#define NORTH_RX_SWITCH_SETUP NORTH_RX_SWITCH_DIR setOut NORTH_RX_SWITCH_PIN
#define NORTH_RX_SWITCH_HI NORTH_RX_SWITCH_OUT setHi NORTH_RX_SWITCH_PIN
#define NORTH_RX_SWITCH_LO NORTH_RX_SWITCH_OUT setLo NORTH_RX_SWITCH_PIN
#define NORTH_RX_SWITCH_IS_HI (0 != (NORTH_RX_SWITCH_IN getBit NORTH_RX_SWITCH_PIN))
#define NORTH_RX_SWITCH_IS_LO (0 == (NORTH_RX_SWITCH_IN getBit NORTH_RX_SWITCH_PIN))

// led
#define LED_PIN Pin3
#define LED_DIR ADir
#define LED_OUT AOut
#define LED_IN AOut

//#define LED_SETUP LED_DIR setOut LED_PIN
#define LED_ON LED_OUT setHi LED_PIN
#define LED_OFF LED_OUT setLo LED_PIN
#define LED_IS_ON (0 != (LED_IN getBit LED_PIN))
#define LED_IS_OFF (0 == (LED_IN getBit LED_PIN))
#define LED_TOGGLE LED_OUT toggleBit LED_PIN

// test point
#define TESTPOINT_PIN Pin7
#define TESTPOINT_DIR ADir
#define TESTPOINT_OUT AOut
#define TESTPOINT_IN AOut

//#define TESTPOINT_SETUP TESTPOINT_DIR setOut TESTPOINT_PIN
#define TESTPOINT_HI TESTPOINT_OUT setHi TESTPOINT_PIN
#define TESTPOINT_LO TESTPOINT_OUT setLo TESTPOINT_PIN
#define TESTPOINT_IS_HI (0 != (TESTPOINT_IN getBit TESTPOINT_PIN))
#define TESTPOINT_IS_LO (0 == (TESTPOINT_IN getBit TESTPOINT_PIN))
#define TESTPOINT_TOGGLE TESTPOINT_OUT toggleBit TESTPOINT_PIN


#ifdef __AVR_ATtiny20__
// is set per default: ADir setIn (NORTH_RX_PIN | SOUTH_RX_PIN)
#  define SETUP_PORT_DIRS ADir setOut (NORTH_TX_PIN | NORTH_RX_SWITCH_PIN | SOUTH_TX_PIN | \
    SOUTH_RX_SWITCH_PIN | LED_PIN | TESTPOINT_PIN)

// is set per default: ADir setLo (LED_PIN | TESTPOINT_PIN)
#  define SETUP_PORT_PINS AOut setHi (NORTH_TX_PIN | NORTH_RX_SWITCH_PIN| SOUTH_TX_PIN  | \
    SOUTH_RX_SWITCH_PIN | NORTH_RX_PIN | SOUTH_RX_PIN)

#elif __AVR_ATmega16__
// is set per default: DDir setIn (NORTH_RX_PIN | SOUTH_RX_PIN)
#  define SETUP_PORT_DIRS ADir setOut (NORTH_TX_PIN | NORTH_RX_SWITCH_PIN | SOUTH_TX_PIN | \
    SOUTH_RX_SWITCH_PIN | LED_PIN | TESTPOINT_PIN)

#  define SETUP_PORT_A_PINS AOut setHi (NORTH_TX_PIN | NORTH_RX_SWITCH_PIN | SOUTH_TX_PIN | \
    SOUTH_RX_SWITCH_PIN)
#  define SETUP_PORT_D_PINS DOut setHi (NORTH_RX_PIN | SOUTH_RX_PIN)
// is set per default: ADir setLo (LED_PIN | TESTPOINT_PIN)
#  define SETUP_PORT_PINS SETUP_PORT_A_PINS; SETUP_PORT_D_PINS
#else
#  error
#endif

#endif