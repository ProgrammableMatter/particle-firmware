/**
 * @author Raoul Rubien 2015
 */

#pragma once

#include "common/PortADefinition.h"
#include "common/PortBDefinition.h"
#include "common/PortCDefinition.h"

#if defined(__AVR_ATmega16__)

#  include "common/PortDDefinition.h"

#endif

#include "common/PortInteraction.h"

/**
 * south tx/rx
 */
// tx south
#define __SOUTH_TX_PIN Pin3
#define __SOUTH_TX_DIR ADir
#define __SOUTH_TX_OUT AOut
#define __SOUTH_TX_IN AIn


#define SOUTH_TX_HI __SOUTH_TX_OUT setHi __SOUTH_TX_PIN
#define SOUTH_TX_LO __SOUTH_TX_OUT setLo __SOUTH_TX_PIN
#define __SOUTH_TX ((unsigned char)((false == (__SOUTH_TX_IN getBit __SOUTH_TX_PIN)) ? false : true))
#define SOUTH_TX_IS_HI (0 != (__SOUTH_TX))
#define SOUTH_TX_IS_LO (false == (__SOUTH_TX))
#define SOUTH_TX_TOGGLE __SOUTH_TX_OUT toggleBit __SOUTH_TX_PIN

#define SOUTH_TX_SETUP __SOUTH_TX_DIR setOut __SOUTH_TX_PIN; SOUTH_TX_HI

// rx south
#ifdef __AVR_ATtiny1634__
#  define __SOUTH_RX_PIN Pin4
#  define __SOUTH_RX_DIR ADir
#  define __SOUTH_RX_OUT AOut
#  define __SOUTH_RX_IN AIn
#else
#  if defined(__AVR_ATmega16__)
#    define __SOUTH_RX_PIN Pin2
#    define __SOUTH_RX_DIR DDir
#    define __SOUTH_RX_OUT DOut
#    define __SOUTH_RX_IN DIn
#  else
#    error
#  endif
#endif

#define SOUTH_RX_PULL_UP __SOUTH_RX_OUT setHi __SOUTH_RX_PIN
#define SOUTH_RX_PULL_DOWN __SOUTH_RX_OUT setLo __SOUTH_RX_PIN
#define __SOUTH_RX ((unsigned char)((false == (__SOUTH_RX_IN getBit __SOUTH_RX_PIN)) ? false : true))
#define SOUTH_RX_IS_HI (__SOUTH_RX)
#define SOUTH_RX_IS_LO (false == (__SOUTH_RX))

#define SOUTH_RX_SETUP __SOUTH_RX_DIR setIn __SOUTH_RX_PIN; SOUTH_RX_PULL_UP

// rx-switch south
#define __SOUTH_RX_SWITCH_PIN Pin2
#define __SOUTH_RX_SWITCH_DIR ADir
#define __SOUTH_RX_SWITCH_OUT AOut
#define __SOUTH_RX_SWITCH_IN AIn


#define SOUTH_RX_SWITCH_HI __SOUTH_RX_SWITCH_OUT setHi __SOUTH_RX_SWITCH_PIN
#define SOUTH_RX_SWITCH_LO __SOUTH_RX_SWITCH_OUT setLo __SOUTH_RX_SWITCH_PIN
#define __SOUTH_RX_SWITCH ((unsigned char)((false == (__SOUTH_RX_IN getBit __SOUTH_RX_PIN)) ? false : true))
#define SOUTH_RX_SWITCH_IS_HI (__SOUTH_RX_SWITCH)
#define SOUTH_RX_SWITCH_IS_LO (false == (__SOUTH_RX_SWITCH))

#define SOUTH_RX_SWITCH_SETUP __SOUTH_RX_SWITCH_DIR setHi __SOUTH_RX_SWITCH_PIN; SOUTH_RX_SWITCH_HI

/**
 * north tx/rx
 */
// tx north
#define __NORTH_TX_PIN Pin0
#define __NORTH_TX_DIR CDir
#define __NORTH_TX_OUT COut
#define __NORTH_TX_IN CIn

#define NORTH_TX_HI __NORTH_TX_OUT setHi __NORTH_TX_PIN
#define NORTH_TX_LO __NORTH_TX_OUT setLo __NORTH_TX_PIN
#define __NORTH_TX ((unsigned char)((false == (__NORTH_TX_IN getBit __NORTH_TX_PIN)) ? false : true))
#define NORTH_TX_IS_HI (__NORTH_TX)
#define NORTH_TX_IS_LO (false == (__NORTH_TX))
#define NORTH_TX_TOGGLE __NORTH_TX_OUT toggleBit __NORTH_TX_PIN

#define NORTH_TX_SETUP __NORTH_TX_DIR setOut __NORTH_TX_PIN; NORTH_TX_HI

// rx north
#ifdef __AVR_ATtiny1634__
#  define __NORTH_RX_PIN Pin5
#  define __NORTH_RX_DIR CDir
#  define __NORTH_RX_OUT COut
#  define __NORTH_RX_IN CIn
#else
#  if defined(__AVR_ATmega16__)
#    define __NORTH_RX_PIN Pin2
#    define __NORTH_RX_DIR BDir
#    define __NORTH_RX_OUT BOut
#    define __NORTH_RX_IN BIn
#  else
#    error
#  endif
#endif

#define NORTH_RX_PULL_UP __NORTH_RX_OUT setHi __NORTH_RX_PIN
#define NORTH_RX_PULL_DOWN __NORTH_RX_OUT setLo __NORTH_RX_PIN
#define __NORTH_RX ((unsigned char)((false == (__NORTH_RX_IN getBit __NORTH_RX_PIN)) ? false : true))
#define NORTH_RX_IS_HI (__NORTH_RX)
#define NORTH_RX_IS_LO (false == (__NORTH_RX))

#define NORTH_RX_SETUP __NORTH_RX_DIR setIn __NORTH_RX_PIN; NORTH_RX_PULL_UP

// rx-switch north
#define __NORTH_RX_SWITCH_PIN Pin4
#define __NORTH_RX_SWITCH_DIR CDir
#define __NORTH_RX_SWITCH_OUT COut
#define __NORTH_RX_SWITCH_IN CIn

#define NORTH_RX_SWITCH_HI (__NORTH_RX_SWITCH_OUT setHi __NORTH_RX_SWITCH_PIN)
#define NORTH_RX_SWITCH_LO (__NORTH_RX_SWITCH_OUT setLo __NORTH_RX_SWITCH_PIN)
#define __NORTH_RX_SWITCH ((unsigned char)((false == (__NORTH_RX_SWITCH_IN getBit __NORTH_RX_SWITCH_PIN)) ? false : true))
#define NORTH_RX_SWITCH_IS_HI (__NORTH_RX_SWITCH)
#define NORTH_RX_SWITCH_IS_LO (false == (__NORTH_RX_SWITCH))
#define NORTH_RX_SWITCH_TOGGLE (__NORTH_RX_SWITCH_OUT toggleBit __NORTH_RX_SWITCH_PIN)
#define NORTH_RX_SWITCH_SETUP __NORTH_RX_SWITCH_DIR setOut __NORTH_RX_SWITCH_PIN; NORTH_RX_SWITCH_HI

/**
 * east tx/rx
 */
// tx east
#define __EAST_TX_PIN Pin7
#define __EAST_TX_DIR ADir
#define __EAST_TX_OUT AOut
#define __EAST_TX_IN AIn

#define EAST_TX_HI __EAST_TX_OUT setHi __EAST_TX_PIN
#define EAST_TX_LO __EAST_TX_OUT setLo __EAST_TX_PIN
#define __EAST_TX ((unsigned char)((false == (__EAST_TX_IN getBit __EAST_TX_PIN)) ? false : true))
#define EAST_TX_IS_HI (__EAST_TX)
#define EAST_TX_IS_LO (false == (__EAST_TX))
#define EAST_TX_TOGGLE __EAST_TX_OUT toggleBit __EAST_TX_PIN

#define EAST_TX_SETUP __EAST_TX_DIR setOut __EAST_TX_PIN; EAST_TX_HI

// rx east
#ifdef __AVR_ATtiny1634__
#  define __EAST_RX_PIN Pin0
#  define __EAST_RX_DIR BDir
#  define __EAST_RX_OUT BOut
#  define __EAST_RX_IN BIn
#else
#  if defined(__AVR_ATmega16__)
#    define __EAST_RX_PIN Pin3
#    define __EAST_RX_DIR DDir
#    define __EAST_RX_OUT DOut
#    define __EAST_RX_IN DIn
#  else
#    error
#  endif
#endif

#define EAST_RX_PULL_UP __EAST_RX_OUT setHi __EAST_RX_PIN
#define EAST_RX_PULL_DOWN __EAST_RX_OUT setLo __EAST_RX_PIN
#define __EAST_RX ((unsigned char)((false == (__EAST_RX_IN getBit __EAST_RX_PIN)) ? false : true))
#define EAST_RX_IS_HI (__EAST_RX)
#define EAST_RX_IS_LO (false == (__EAST_RX))

#define EAST_RX_SETUP __EAST_RX_DIR setIn __EAST_RX_PIN; EAST_RX_PULL_UP

// rx-switch east
#define __EAST_RX_SWITCH_PIN Pin6
#define __EAST_RX_SWITCH_DIR ADir
#define __EAST_RX_SWITCH_OUT AOut
#define __EAST_RX_SWITCH_IN AIn

#define EAST_RX_SWITCH_HI __EAST_RX_SWITCH_OUT setHi __EAST_RX_SWITCH_PIN
#define EAST_RX_SWITCH_LO __EAST_RX_SWITCH_OUT setLo __EAST_RX_SWITCH_PIN
#define __EAST_RX_SWITCH ((unsigned char)((false == (__EAST_RX_SWITCH_IN getBit __EAST_RX_SWITCH_PIN)) ? false : true))
#define EAST_RX_SWITCH_IS_HI (__EAST_RX_SWITCH)
#define EAST_RX_SWITCH_IS_LO (false == (__EAST_RX_SWITCH))

#define EAST_RX_SWITCH_SETUP __EAST_RX_SWITCH_DIR setOut __EAST_RX_SWITCH_PIN; EAST_RX_SWITCH_HI

/**
 * LEDs
 */
// error led
#define __LED_ERROR_PIN Pin0
#define __LED_ERROR_DIR ADir
#define __LED_ERROR_OUT AOut
#define __LED_ERROR_IN AIn

#define LED_ERROR __LED_ERROR_IN getBit __LED_ERROR_PIN
#define LED_ERROR_ON __LED_ERROR_OUT setHi __LED_ERROR_PIN
#define LED_ERROR_OFF __LED_ERROR_OUT setLo __LED_ERROR_PIN
#define LED_ERROR_IS_ON (0 != (LED_ERROR))
#define LED_ERROR_IS_OFF (false == (LED_ERROR))
#define LED_ERROR_TOGGLE __LED_ERROR_OUT toggleBit __LED_ERROR_PIN

#define LED_ERROR_SETUP __LED_ERROR_DIR setOut __LED_ERROR_PIN; LED_ERROR_ON

// status led 1
#define __LED_STATUS1_PIN Pin3
#define __LED_STATUS1_DIR BDir
#define __LED_STATUS1_OUT BOut
#define __LED_STATUS1_IN BIn

#define LED_STATUS1 __LED_STATUS1_IN getBit __LED_STATUS1_PIN
#define LED_STATUS1_ON __LED_STATUS1_OUT setHi __LED_STATUS1_PIN
#define LED_STATUS1_OFF __LED_STATUS1_OUT setLo __LED_STATUS1_PIN
#define LED_STATUS1_IS_ON (0 != (LED_STATUS1))
#define LED_STATUS1_IS_OFF (false == (LED_STATUS1))
#define LED_STATUS1_TOGGLE __LED_STATUS1_OUT toggleBit __LED_STATUS1_PIN

#define LED_STATUS1_SETUP __LED_STATUS1_DIR setOut __LED_STATUS1_PIN; LED_STATUS1_ON

// status led 0
#ifdef __AVR_ATtiny1634__
#  define __LED_STATUS0_PIN Pin2
#  define __LED_STATUS0_DIR BDir
#  define __LED_STATUS0_OUT BOut
#  define __LED_STATUS0_IN BIn
#else
#  if defined(__AVR_ATmega16__)
#    define __LED_STATUS0_PIN Pin4
#    define __LED_STATUS0_DIR BDir
#    define __LED_STATUS0_OUT BOut
#    define __LED_STATUS0_IN BIn
#  else
#    error
#  endif
#endif


#define LED_STATUS0 __LED_STATUS0_IN getBit __LED_STATUS0_PIN
#define LED_STATUS0_ON __LED_STATUS0_OUT setHi __LED_STATUS0_PIN
#define LED_STATUS0_OFF __LED_STATUS0_OUT setLo __LED_STATUS0_PIN
#define LED_STATUS0_IS_ON (0 != (LED_STATUS0))
#define LED_STATUS0_IS_OFF (false == (LED_STATUS0))
#define LED_STATUS0_TOGGLE __LED_STATUS0_OUT toggleBit __LED_STATUS0_PIN

#define LED_STATUS0_SETUP __LED_STATUS0_DIR setOut __LED_STATUS0_PIN; LED_STATUS0_ON

// heartbeat led
#define __LED_HEARTBEAT_PIN Pin1
#define __LED_HEARTBEAT_DIR BDir
#define __LED_HEARTBEAT_OUT BOut
#define __LED_HEARTBEAT_IN BIn

#define LED_HEARTBEAT __LED_HEARTBEAT_IN getBit __LED_HEARTBEAT_PIN
#define LED_HEARTBEAT_ON __LED_HEARTBEAT_OUT setHi __LED_HEARTBEAT_PIN
#define LED_HEARTBEAT_OFF __LED_HEARTBEAT_OUT setLo __LED_HEARTBEAT_PIN
#define LED_HEARTBEAT_IS_ON (0 != (LED_HEARTBEAT))
#define LED_HEARTBEAT_IS_OFF (false == (LED_HEARTBEAT))
#define LED_HEARTBEAT_TOGGLE __LED_HEARTBEAT_OUT toggleBit __LED_HEARTBEAT_PIN

#define LED_HEARTBEAT_SETUP __LED_HEARTBEAT_DIR setOut __LED_HEARTBEAT_PIN; LED_HEARTBEAT_ON

/**
 * test points
 */
// test point 1
#define __TEST_POINT1_PIN Pin2
#define __TEST_POINT1_DIR CDir
#define __TEST_POINT1_OUT COut
#define __TEST_POINT1_IN CIn

#define TEST_POINT1 __TEST_POINT1_IN getBit __TEST_POINT1_PIN
#define TEST_POINT1_HI __TEST_POINT1_OUT setHi __TEST_POINT1_PIN
#define TEST_POINT1_LO __TEST_POINT1_OUT setLo __TEST_POINT1_PIN
#define TEST_POINT1_IS_HI (0 != (TEST_POINT1))
#define TEST_POINT1_IS_LO (false == (TEST_POINT1))
#define TEST_POINT1_TOGGLE __TEST_POINT1_OUT toggleBit __TEST_POINT1_PIN

#define TEST_POINT1_SETUP __TEST_POINT1_DIR setOut __TEST_POINT1_PIN; TEST_POINT1_HI

// test point 2
#define __TEST_POINT2_PIN Pin1
#define __TEST_POINT2_DIR ADir
#define __TEST_POINT2_OUT AOut
#define __TEST_POINT2_IN AIn

#define TEST_POINT2 __TEST_POINT2_IN getBit __TEST_POINT2_PIN
#define TEST_POINT2_HI __TEST_POINT2_OUT setHi __TEST_POINT2_PIN
#define TEST_POINT2_LO __TEST_POINT2_OUT setLo __TEST_POINT2_PIN
#define TEST_POINT2_IS_HI (0 != (TEST_POINT2))
#define TEST_POINT2_IS_LO (false == (TEST_POINT2))
#define TEST_POINT2_TOGGLE __TEST_POINT2_OUT toggleBit __TEST_POINT2_PIN

#define TEST_POINT2_SETUP __TEST_POINT2_DIR setOut __TEST_POINT2_PIN; TEST_POINT2_HI

// test point 3
#define __TEST_POINT3_PIN Pin5
#define __TEST_POINT3_DIR ADir
#define __TEST_POINT3_OUT AOut
#define __TEST_POINT3_IN AIn

#define TEST_POINT3 __TEST_POINT3_IN getBit __TEST_POINT3_PIN
#define TEST_POINT3_HI __TEST_POINT3_OUT setHi __TEST_POINT3_PIN
#define TEST_POINT3_LO __TEST_POINT3_OUT setLo __TEST_POINT3_PIN
#define TEST_POINT3_IS_HI (0 != (TEST_POINT3))
#define TEST_POINT3_IS_LO (false == (TEST_POINT3))
#define TEST_POINT3_TOGGLE __TEST_POINT3_OUT toggleBit __TEST_POINT3_PIN

#define TEST_POINT3_SETUP __TEST_POINT3_DIR setOut __TEST_POINT3_PIN; TEST_POINT3_HI

/**
 * setup
 */
#define IO_PORTS_SETUP \
NORTH_RX_SETUP; NORTH_TX_SETUP; NORTH_RX_SWITCH_SETUP; \
SOUTH_RX_SETUP; SOUTH_TX_SETUP; SOUTH_RX_SWITCH_SETUP; \
EAST_RX_SETUP; EAST_TX_SETUP; EAST_RX_SWITCH_SETUP; \
LED_ERROR_SETUP; LED_STATUS0_SETUP; LED_STATUS1_SETUP; LED_HEARTBEAT_SETUP; \
TEST_POINT1_SETUP; TEST_POINT2_SETUP; TEST_POINT3_SETUP;
