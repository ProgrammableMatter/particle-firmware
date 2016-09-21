/**
 * @author Raoul Rubien 2016
 *
 * Input and output pin related arguments.
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
#  define __SOUTH_RX_IN  AIn
#  define SOUTH_RX_PULL_UP PUEA pullUp __SOUTH_RX_PIN
#  define SOUTH_RX_PULL_UP_DISABLE PUEA setLo __SOUTH_RX_PIN
#else
#  if defined(__AVR_ATmega16__)
#    define __SOUTH_RX_PIN Pin2
#    define __SOUTH_RX_DIR DDir
#    define __SOUTH_RX_OUT DOut
#    define __SOUTH_RX_IN  DIn
#    define SOUTH_RX_PULL_UP_DISABLE __SOUTH_RX_OUT setLo __SOUTH_RX_PIN
#    define SOUTH_RX_PULL_UP __SOUTH_RX_OUT pullUp __SOUTH_RX_PIN
#  else
#    error
#  endif
#endif

#define __SOUTH_RX ((unsigned char)((false == (__SOUTH_RX_IN getBit __SOUTH_RX_PIN)) ? false : true))
#define SOUTH_RX_IS_HI (__SOUTH_RX)
#define SOUTH_RX_IS_LO (false == (__SOUTH_RX))

#define SOUTH_RX_SETUP __SOUTH_RX_DIR setIn __SOUTH_RX_PIN; SOUTH_RX_PULL_UP

// rx-switch south
#define __SOUTH_RX_SWITCH_PIN Pin2
#define __SOUTH_RX_SWITCH_DIR ADir
#define __SOUTH_RX_SWITCH_OUT AOut
#define __SOUTH_RX_SWITCH_IN  AIn


#define SOUTH_RX_SWITCH_HI __SOUTH_RX_SWITCH_OUT setHi __SOUTH_RX_SWITCH_PIN
#define SOUTH_RX_SWITCH_LO __SOUTH_RX_SWITCH_OUT setLo __SOUTH_RX_SWITCH_PIN
#define __SOUTH_RX_SWITCH ((unsigned char)((false == (__SOUTH_RX_IN getBit __SOUTH_RX_PIN)) ? false : true))
#define SOUTH_RX_SWITCH_IS_HI (__SOUTH_RX_SWITCH)
#define SOUTH_RX_SWITCH_IS_LO (false == (__SOUTH_RX_SWITCH))

#define SOUTH_RX_SWITCH_SETUP __SOUTH_RX_SWITCH_DIR setOut __SOUTH_RX_SWITCH_PIN; SOUTH_RX_SWITCH_HI

/**
 * north tx/rx
 */
// tx north
#define __NORTH_TX_PIN Pin0
#define __NORTH_TX_DIR CDir
#define __NORTH_TX_OUT COut
#define __NORTH_TX_IN  CIn

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
#  define __NORTH_RX_IN  CIn
#  define NORTH_RX_PULL_UP PUEC pullUp __NORTH_RX_PIN
#  define NORTH_RX_PULL_UP_DISABLE PUEC setLo __NORTH_RX_PIN
#else
#  if defined(__AVR_ATmega16__)
#    define __NORTH_RX_PIN Pin2
#    define __NORTH_RX_DIR BDir
#    define __NORTH_RX_OUT BOut
#    define __NORTH_RX_IN BIn
#    define NORTH_RX_PULL_UP __NORTH_RX_OUT pullUp __NORTH_RX_PIN
#    define NORTH_RX_PULL_UP_DISABLE __NORTH_RX_OUT setLo __NORTH_RX_PIN
#  else
#    error
#  endif
#endif

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
#define __EAST_TX_IN  AIn

#define EAST_TX_HI __EAST_TX_OUT setHi __EAST_TX_PIN
#define EAST_TX_LO __EAST_TX_OUT setLo __EAST_TX_PIN
#define __EAST_TX ((unsigned char)((false == (__EAST_TX_IN getBit __EAST_TX_PIN)) ? false : true))
#define EAST_TX_IS_HI (__EAST_TX)
#define EAST_TX_IS_LO (false == (__EAST_TX))
#define EAST_TX_TOGGLE __EAST_TX_OUT toggleBit __EAST_TX_PIN

#define EAST_TX_SETUP __EAST_TX_DIR setOut __EAST_TX_PIN; EAST_TX_LO

// rx east
#ifdef __AVR_ATtiny1634__
#  define __EAST_RX_PIN Pin0
#  define __EAST_RX_DIR BDir
#  define __EAST_RX_OUT BOut
#  define __EAST_RX_IN BIn
#  define EAST_RX_PULL_UP PUEB pullUp __EAST_RX_PIN
#  define EAST_RX_PULL_UP_DISABLE PUEB setLo __EAST_RX_PIN
#else
#  if defined(__AVR_ATmega16__)
#    define __EAST_RX_PIN Pin3
#    define __EAST_RX_DIR DDir
#    define __EAST_RX_OUT DOut
#    define __EAST_RX_IN DIn
#  define EAST_RX_PULL_UP __EAST_RX_OUT pullUp __EAST_RX_PIN
#  define EAST_RX_PULL_UP_DISABLE __EAST_RX_OUT setLo __EAST_RX_PIN
#  else
#    error
#  endif
#endif

#define __EAST_RX ((unsigned char)((false == (__EAST_RX_IN getBit __EAST_RX_PIN)) ? false : true))
#define EAST_RX_IS_HI (__EAST_RX)
#define EAST_RX_IS_LO (false == (__EAST_RX))

#define EAST_RX_SETUP __EAST_RX_DIR setIn __EAST_RX_PIN; EAST_RX_PULL_UP

/**
 * LEDs
 */
// status led 1
#define __LED_STATUS1_PIN Pin3
#define __LED_STATUS1_DIR BDir
#define __LED_STATUS1_OUT BOut
#define __LED_STATUS1_IN  BIn

#define LED_STATUS1        __LED_STATUS1_IN  getBit    __LED_STATUS1_PIN
#define LED_STATUS1_ON     __LED_STATUS1_OUT setHi     __LED_STATUS1_PIN
#define LED_STATUS1_OFF    __LED_STATUS1_OUT setLo     __LED_STATUS1_PIN
#define LED_STATUS1_IS_ON      (0 != (LED_STATUS1))
#define LED_STATUS1_IS_OFF (false == (LED_STATUS1))
#define LED_STATUS1_TOGGLE __LED_STATUS1_OUT toggleBit __LED_STATUS1_PIN

#define LED_STATUS1_SETUP  __LED_STATUS1_DIR setOut    __LED_STATUS1_PIN; LED_STATUS1_OFF

// status led 2
#define __LED_STATUS2_PIN Pin0
#define __LED_STATUS2_DIR ADir
#define __LED_STATUS2_OUT AOut
#define __LED_STATUS2_IN  AIn

#define LED_STATUS2        __LED_STATUS2_IN  getBit    __LED_STATUS2_PIN
#define LED_STATUS2_ON     __LED_STATUS2_OUT setHi     __LED_STATUS2_PIN
#define LED_STATUS2_OFF    __LED_STATUS2_OUT setLo     __LED_STATUS2_PIN
#define LED_STATUS2_IS_ON      (0 != (LED_STATUS2))
#define LED_STATUS2_IS_OFF (false == (LED_STATUS2))
#define LED_STATUS2_TOGGLE __LED_STATUS2_OUT toggleBit __LED_STATUS2_PIN

#define LED_STATUS2_SETUP  __LED_STATUS2_DIR setOut    __LED_STATUS2_PIN; LED_STATUS2_OFF

// status led 3
#define __LED_STATUS3_PIN Pin1
#define __LED_STATUS3_DIR ADir
#define __LED_STATUS3_OUT AOut
#define __LED_STATUS3_IN  AIn

#define LED_STATUS3        __LED_STATUS3_IN  getBit    __LED_STATUS3_PIN
#define LED_STATUS3_ON     __LED_STATUS3_OUT setHi     __LED_STATUS3_PIN
#define LED_STATUS3_OFF    __LED_STATUS3_OUT setLo     __LED_STATUS3_PIN
#define LED_STATUS3_IS_ON      (0 != (LED_STATUS3))
#define LED_STATUS3_IS_OFF (false == (LED_STATUS3))
#define LED_STATUS3_TOGGLE __LED_STATUS3_OUT toggleBit __LED_STATUS3_PIN

#define LED_STATUS3_SETUP  __LED_STATUS3_DIR setOut    __LED_STATUS3_PIN; LED_STATUS3_OFF


// status led 4
#define __LED_STATUS4_PIN Pin6
#define __LED_STATUS4_DIR ADir
#define __LED_STATUS4_OUT AOut
#define __LED_STATUS4_IN  AIn

#define LED_STATUS4        __LED_STATUS4_IN  getBit    __LED_STATUS4_PIN
#define LED_STATUS4_ON     __LED_STATUS4_OUT setHi     __LED_STATUS4_PIN
#define LED_STATUS4_OFF    __LED_STATUS4_OUT setLo     __LED_STATUS4_PIN
#define LED_STATUS4_IS_ON      (0 != (LED_STATUS4))
#define LED_STATUS4_IS_OFF (false == (LED_STATUS4))
#define LED_STATUS4_TOGGLE __LED_STATUS4_OUT toggleBit __LED_STATUS4_PIN

#define LED_STATUS4_SETUP  __LED_STATUS4_DIR setOut    __LED_STATUS4_PIN; LED_STATUS4_OFF

// status led 5
#define __LED_STATUS5_PIN Pin5
#define __LED_STATUS5_DIR ADir
#define __LED_STATUS5_OUT AOut
#define __LED_STATUS5_IN  AIn

#define LED_STATUS5        __LED_STATUS5_IN  getBit    __LED_STATUS5_PIN
#define LED_STATUS5_ON     __LED_STATUS5_OUT setHi     __LED_STATUS5_PIN
#define LED_STATUS5_OFF    __LED_STATUS5_OUT setLo     __LED_STATUS5_PIN
#define LED_STATUS5_IS_ON      (0 != (LED_STATUS5))
#define LED_STATUS5_IS_OFF (false == (LED_STATUS5))
#define LED_STATUS5_TOGGLE __LED_STATUS5_OUT toggleBit __LED_STATUS5_PIN

#define LED_STATUS5_SETUP  __LED_STATUS5_DIR setOut    __LED_STATUS5_PIN; LED_STATUS5_OFF

// status led 6
#define __LED_STATUS6_PIN Pin2
#define __LED_STATUS6_DIR CDir
#define __LED_STATUS6_OUT COut
#define __LED_STATUS6_IN  CIn

#define LED_STATUS6        __LED_STATUS6_IN  getBit    __LED_STATUS6_PIN
#define LED_STATUS6_ON     __LED_STATUS6_OUT setHi     __LED_STATUS6_PIN
#define LED_STATUS6_OFF    __LED_STATUS6_OUT setLo     __LED_STATUS6_PIN
#define LED_STATUS6_IS_ON      (0 != (LED_STATUS6))
#define LED_STATUS6_IS_OFF (false == (LED_STATUS6))
#define LED_STATUS6_TOGGLE __LED_STATUS6_OUT toggleBit __LED_STATUS6_PIN

#define LED_STATUS6_SETUP  __LED_STATUS6_DIR setOut    __LED_STATUS6_PIN; LED_STATUS6_OFF




/**
 * Test points
 */

// test point 1
#ifdef __AVR_ATtiny1634__
#define __TEST_POINT1_PIN     Pin2
#define __TEST_POINT1_DIR     BDir
#define __TEST_POINT1_OUT     BOut
#define __TEST_POINT1_IN      BIn
#else
#  if defined(__AVR_ATmega16__)
#    define __TEST_POINT1_PIN Pin4
#    define __TEST_POINT1_DIR BDir
#    define __TEST_POINT1_OUT BOut
#    define __TEST_POINT1_IN  BIn
#  else
#    error
#  endif
#endif

#define TEST_POINT1         __TEST_POINT1_IN  getBit    __TEST_POINT1_PIN
#define TEST_POINT1_HI      __TEST_POINT1_OUT setHi     __TEST_POINT1_PIN
#define TEST_POINT1_LO      __TEST_POINT1_OUT setLo     __TEST_POINT1_PIN
#define TEST_POINT1_IS_HI     (0 != (TEST_POINT1))
#define TEST_POINT1_IS_LO (false == (TEST_POINT1))
#define TEST_POINT1_TOGGLE  __TEST_POINT1_OUT toggleBit __TEST_POINT1_PIN

#define TEST_POINT1_SETUP   __TEST_POINT1_DIR setOut    __TEST_POINT1_PIN; TEST_POINT1_LO

// test point 2
#define __TEST_POINT2_PIN     Pin1
#define __TEST_POINT2_DIR     BDir
#define __TEST_POINT2_OUT     BOut
#define __TEST_POINT2_IN      BIn

#define TEST_POINT2         __TEST_POINT2_IN  getBit    __TEST_POINT2_PIN
#define TEST_POINT2_HI      __TEST_POINT2_OUT setHi     __TEST_POINT2_PIN
#define TEST_POINT2_LO      __TEST_POINT2_OUT setLo     __TEST_POINT2_PIN
#define TEST_POINT2_IS_HI     (0 != (TEST_POINT2))
#define TEST_POINT2_IS_LO (false == (TEST_POINT2))
#define TEST_POINT2_TOGGLE  __TEST_POINT2_OUT toggleBit __TEST_POINT2_PIN

#define TEST_POINT2_SETUP   __TEST_POINT2_DIR setOut    __TEST_POINT2_PIN; TEST_POINT2_LO

// unused pin
#define __UNUSED1_PIN     Pin1
#define __UNUSED1_DIR     CDir
#define __UNUSED1_OUT     COut
#define __UNUSED1_IN      CIn

#define __UNUSED1         __UNUSED1_IN  getBit    __UNUSED1_PIN
#define __UNUSED1_HI      __UNUSED1_OUT setHi     __UNUSED1_PIN
#define __UNUSED1_LO      __UNUSED1_OUT setLo     __UNUSED1_PIN
#define __UNUSED1_IS_HI     (0 != (__UNUSED1))
#define __UNUSED1_IS_LO (false == (__UNUSED1))
#define __UNUSED1_TOGGLE  __UNUSED1_OUT toggleBit __UNUSED1_PIN

#define __UNUSED1_SETUP   __UNUSED1_DIR setOut    __UNUSED1_PIN; __UNUSED1_LO


/**
 * setup
 */
#define IO_PORTS_SETUP \
NORTH_RX_SETUP; NORTH_TX_SETUP; NORTH_RX_SWITCH_SETUP; \
EAST_RX_SETUP;  EAST_TX_SETUP; \
SOUTH_RX_SETUP; SOUTH_TX_SETUP; SOUTH_RX_SWITCH_SETUP; \
LED_STATUS1_SETUP; LED_STATUS2_SETUP; LED_STATUS3_SETUP; \
LED_STATUS4_SETUP; LED_STATUS5_SETUP; LED_STATUS6_SETUP; \
TEST_POINT1_SETUP; TEST_POINT2_SETUP; \
__UNUSED1_SETUP
