/**
 * @author Raoul Rubien 2015
 */

#ifndef PROJECT_PARTICLE_PORT_DEFINITIONS_H
#define PROJECT_PARTICLE_PORT_DEFINITIONS_H

#include <common/PortADefinition.h>
#include <common/PortBDefinition.h>
#include <common/PortCDefinition.h>

#if defined(__AVR_ATmega16__)
#  include <common/PortDDefinition.h>
#endif

#include <common/PortInteraction.h>

/////////////////////// south tx/rx ///////////////////////

// tx south
#define SOUTH_TX_PIN Pin3
#define SOUTH_TX_DIR ADir
#define SOUTH_TX_OUT AOut
#define SOUTH_TX_IN AIn


#define SOUTH_TX_HI SOUTH_TX_OUT setHi SOUTH_TX_PIN
#define SOUTH_TX_LO SOUTH_TX_OUT setLo SOUTH_TX_PIN
#define __SOUTH_TX ((unsigned char)((0 == (SOUTH_TX_IN getBit SOUTH_TX_PIN)) ? 0 : 1))
#define SOUTH_TX_IS_HI (0 != (__SOUTH_TX))
#define SOUTH_TX_IS_LO (0 == (__SOUTH_TX))
#define SOUTH_TX_TOGGLE SOUTH_TX_OUT toggleBit SOUTH_TX_PIN

#define SOUTH_TX_SETUP SOUTH_TX_DIR setOut SOUTH_TX_PIN; SOUTH_TX_HI

// rx south
#ifdef __AVR_ATtiny1634__
#  define SOUTH_RX_PIN Pin4
#  define SOUTH_RX_DIR ADir
#  define SOUTH_RX_OUT AOut
#  define SOUTH_RX_IN AIn
#else
#  if defined(__AVR_ATmega16__)
#    define SOUTH_RX_PIN Pin2
#    define SOUTH_RX_DIR DDir
#    define SOUTH_RX_OUT DOut
#    define SOUTH_RX_IN DIn
#  else
#    error
#  endif
#endif

#define SOUTH_RX_PULL_UP SOUTH_RX_OUT setHi SOUTH_RX_PIN
#define SOUTH_RX_PULL_DOWN SOUTH_RX_OUT setLo SOUTH_RX_PIN
#define __SOUTH_RX ((unsigned char)((0 == (SOUTH_RX_IN getBit SOUTH_RX_PIN)) ? 0 : 1))
#define SOUTH_RX_IS_HI (__SOUTH_RX)
#define SOUTH_RX_IS_LO (0 == __SOUTH_RX)

#define SOUTH_RX_SETUP SOUTH_RX_DIR setIn SOUTH_RX_PIN; SOUTH_RX_PULL_UP

// rx-switch south
#define SOUTH_RX_SWITCH_PIN Pin2
#define SOUTH_RX_SWITCH_DIR ADir
#define SOUTH_RX_SWITCH_OUT AOut
#define SOUTH_RX_SWITCH_IN AIn


#define SOUTH_RX_SWITCH_HI SOUTH_RX_SWITCH_OUT setHi SOUTH_RX_SWITCH_PIN
#define SOUTH_RX_SWITCH_LO SOUTH_RX_SWITCH_OUT setLo SOUTH_RX_SWITCH_PIN
#define __SOUTH_RX_SWITCH ((unsigned char)((0 == (SOUTH_RX_IN getBit SOUTH_RX_PIN)) ? 0 : 1))
#define SOUTH_RX_SWITCH_IS_HI (__SOUTH_RX_SWITCH)
#define SOUTH_RX_SWITCH_IS_LO (0 == __SOUTH_RX_SWITCH)

#define SOUTH_RX_SWITCH_SETUP SOUTH_RX_SWITCH_DIR setHi SOUTH_RX_SWITCH_PIN; SOUTH_RX_SWITCH_HI

/////////////////////// north tx/rx ///////////////////////

// tx north
#define NORTH_TX_PIN Pin0
#define NORTH_TX_DIR CDir
#define NORTH_TX_OUT COut
#define NORTH_TX_IN CIn

#define NORTH_TX_HI NORTH_TX_OUT setHi NORTH_TX_PIN
#define NORTH_TX_LO NORTH_TX_OUT setLo NORTH_TX_PIN
#define __NORTH_TX ((unsigned char)((0 == (NORTH_TX_IN getBit NORTH_TX_PIN)) ? 0 : 1))
#define NORTH_TX_IS_HI (__NORTH_TX)
#define NORTH_TX_IS_LO (0 == __NORTH_TX)
#define NORTH_TX_TOGGLE NORTH_TX_OUT toggleBit NORTH_TX_PIN

#define NORTH_TX_SETUP NORTH_TX_DIR setOut NORTH_TX_PIN; NORTH_TX_HI

// rx north
#ifdef __AVR_ATtiny1634__
#  define NORTH_RX_PIN Pin5
#  define NORTH_RX_DIR CDir
#  define NORTH_RX_OUT COut
#  define NORTH_RX_IN CIn
#else
#  if defined(__AVR_ATmega16__)
#    define NORTH_RX_PIN Pin2
#    define NORTH_RX_DIR BDir
#    define NORTH_RX_OUT BOut
#    define NORTH_RX_IN BIn
#  else
#    error
#  endif
#endif

#define NORTH_RX_PULL_UP NORTH_RX_OUT setHi NORTH_RX_PIN
#define NORTH_RX_PULL_DOWN NORTH_RX_OUT setLo NORTH_RX_PIN
#define __NORTH_RX ((unsigned char)((0 == (NORTH_RX_IN getBit NORTH_RX_PIN)) ? 0 : 1))
#define NORTH_RX_IS_HI (__NORTH_RX)
#define NORTH_RX_IS_LO (0 == __NORTH_RX)

#define NORTH_RX_SETUP NORTH_RX_DIR setIn NORTH_RX_PIN; NORTH_RX_PULL_UP

// rx-switch north
#define NORTH_RX_SWITCH_PIN Pin4
#define NORTH_RX_SWITCH_DIR CDir
#define NORTH_RX_SWITCH_OUT COut
#define NORTH_RX_SWITCH_IN CIn

#define NORTH_RX_SWITCH_HI NORTH_RX_SWITCH_OUT setHi NORTH_RX_SWITCH_PIN
#define NORTH_RX_SWITCH_LO NORTH_RX_SWITCH_OUT setLo NORTH_RX_SWITCH_PIN
#define __NORTH_RX_SWITCH ((unsigned char)((0 == (NORTH_RX_SWITCH_IN getBit NORTH_RX_SWITCH_PIN)) ? 0 : 1))
#define NORTH_RX_SWITCH_IS_HI (__NORTH_RX_SWITCH)
#define NORTH_RX_SWITCH_IS_LO (0 == __NORTH_RX_SWITCH)

#define NORTH_RX_SWITCH_SETUP NORTH_RX_SWITCH_DIR setOut NORTH_RX_SWITCH_PIN; NORTH_RX_SWITCH_HI

/////////////////////// east tx/rx ///////////////////////

// tx east
#define EAST_TX_PIN Pin7
#define EAST_TX_DIR ADir
#define EAST_TX_OUT AOut
#define EAST_TX_IN AIn

#define EAST_TX_HI EAST_TX_OUT setHi EAST_TX_PIN
#define EAST_TX_LO EAST_TX_OUT setLo EAST_TX_PIN
#define __EAST_TX ((unsigned char)((0 == (EAST_TX_IN getBit EAST_TX_PIN)) ? 0 : 1))
#define EAST_TX_IS_HI (__EAST_TX)
#define EAST_TX_IS_LO (0 == __EAST_TX)
#define EAST_TX_TOGGLE EAST_TX_OUT toggleBit EAST_TX_PIN

#define EAST_TX_SETUP EAST_TX_DIR setOut EAST_TX_PIN; EAST_TX_HI

// rx east
#ifdef __AVR_ATtiny1634__
#  define EAST_RX_PIN Pin0
#  define EAST_RX_DIR BDir
#  define EAST_RX_OUT BOut
#  define EAST_RX_IN BIn
#else
#  if defined(__AVR_ATmega16__)
#    define EAST_RX_PIN Pin3
#    define EAST_RX_DIR DDir
#    define EAST_RX_OUT DOut
#    define EAST_RX_IN DIn
#  else
#    error
#  endif
#endif

#define EAST_RX_PULL_UP EAST_RX_OUT setHi EAST_RX_PIN
#define EAST_RX_PULL_DOWN EAST_RX_OUT setLo EAST_RX_PIN
#define __EAST_RX ((unsigned char)((0 == (EAST_RX_IN getBit EAST_RX_PIN)) ? 0 : 1))
#define EAST_RX_IS_HI (__EAST_RX)
#define EAST_RX_IS_LO (0 == __EAST_RX)

#define EAST_RX_SETUP EAST_RX_DIR setIn EAST_RX_PIN; EAST_RX_PULL_UP

// rx-switch east
#define EAST_RX_SWITCH_PIN Pin6
#define EAST_RX_SWITCH_DIR ADir
#define EAST_RX_SWITCH_OUT AOut
#define EAST_RX_SWITCH_IN AIn

#define EAST_RX_SWITCH_HI EAST_RX_SWITCH_OUT setHi EAST_RX_SWITCH_PIN
#define EAST_RX_SWITCH_LO EAST_RX_SWITCH_OUT setLo EAST_RX_SWITCH_PIN
#define __EAST_RX_SWITCH ((unsigned char)((0 == (EAST_RX_SWITCH_IN getBit EAST_RX_SWITCH_PIN)) ? 0 : 1))
#define EAST_RX_SWITCH_IS_HI (__EAST_RX_SWITCH)
#define EAST_RX_SWITCH_IS_LO (0 == __EAST_RX_SWITCH)

#define EAST_RX_SWITCH_SETUP EAST_RX_SWITCH_DIR setOut EAST_RX_SWITCH_PIN; EAST_RX_SWITCH_HI

/////////////////////// leds ///////////////////////

// error led
#define LED_ERROR_PIN Pin0
#define LED_ERROR_DIR ADir
#define LED_ERROR_OUT AOut
#define LED_ERROR_IN AIn

#define LED_ERROR LED_ERROR_IN getBit LED_ERROR_PIN
#define LED_ERROR_ON LED_ERROR_OUT setHi LED_ERROR_PIN
#define LED_ERROR_OFF LED_ERROR_OUT setLo LED_ERROR_PIN
#define LED_ERROR_IS_ON (0 != (LED_ERROR))
#define LED_ERROR_IS_OFF (0 == (LED_ERROR))
#define LED_ERROR_TOGGLE LED_ERROR_OUT toggleBit LED_ERROR_PIN

#define LED_ERROR_SETUP LED_ERROR_DIR setOut LED_ERROR_PIN; LED_ERROR_ON

// status led 1
#define LED_STATUS1_PIN Pin3
#define LED_STATUS1_DIR BDir
#define LED_STATUS1_OUT BOut
#define LED_STATUS1_IN BIn

#define LED_STATUS1 LED_STATUS1_IN getBit LED_STATUS1_PIN
#define LED_STATUS1_ON LED_STATUS1_OUT setHi LED_STATUS1_PIN
#define LED_STATUS1_OFF LED_STATUS1_OUT setLo LED_STATUS1_PIN
#define LED_STATUS1_IS_ON (0 != (LED_STATUS1))
#define LED_STATUS1_IS_OFF (0 == (LED_STATUS1))
#define LED_STATUS1_TOGGLE _OUT toggleBit LED_STATUS1_PIN

#define LED_STATUS1_SETUP LED_STATUS1_DIR setOut LED_STATUS1_PIN; LED_STATUS1_ON

// status led 0
#ifdef __AVR_ATtiny1634__
#  define LED_STATUS0_PIN Pin2
#  define LED_STATUS0_DIR BDir
#  define LED_STATUS0_OUT BOut
#  define LED_STATUS0_IN BIn
#else
#  if defined(__AVR_ATmega16__)
#    define LED_STATUS0_PIN Pin4
#    define LED_STATUS0_DIR BDir
#    define LED_STATUS0_OUT BOut
#    define LED_STATUS0_IN BIn
#  else
#    error
#  endif
#endif


#define LED_STATUS0 LED_STATUS0_IN getBit LED_STATUS0_PIN
#define LED_STATUS0_ON LED_STATUS0_OUT setHi LED_STATUS0_PIN
#define LED_STATUS0_OFF LED_STATUS0_OUT setLo LED_STATUS0_PIN
#define LED_STATUS0_IS_ON (0 != (LED_STATUS0))
#define LED_STATUS0_IS_OFF (0 == (LED_STATUS0))
#define LED_STATUS0_TOGGLE LED_STATUS0_OUT toggleBit LED_STATUS0_PIN

#define LED_STATUS0_SETUP LED_STATUS0_DIR setOut LED_STATUS0_PIN; LED_STATUS0_ON

// heartbeat led
#define LED_HEARTBEAT_PIN Pin1
#define LED_HEARTBEAT_DIR BDir
#define LED_HEARTBEAT_OUT BOut
#define LED_HEARTBEAT_IN BIn

#define LED_HEARTBEAT LED_HEARTBEAT_IN getBit LED_HEARTBEAT_PIN
#define LED_HEARTBEAT_ON LED_HEARTBEAT_OUT setHi LED_HEARTBEAT_PIN
#define LED_HEARTBEAT_OFF LED_HEARTBEAT_OUT setLo LED_HEARTBEAT_PIN
#define LED_HEARTBEAT_IS_ON (0 != (LED_HEARTBEAT))
#define LED_HEARTBEAT_IS_OFF (0 == (LED_HEARTBEAT))
#define LED_HEARTBEAT_TOGGLE LED_HEARTBEAT_OUT toggleBit LED_HEARTBEAT_PIN

#define LED_HEARTBEAT_SETUP LED_HEARTBEAT_DIR setOut LED_HEARTBEAT_PIN; LED_HEARTBEAT_ON

/////////////////////// test points ///////////////////////

// test point 1
#define TEST_POINT1_PIN Pin2
#define TEST_POINT1_DIR CDir
#define TEST_POINT1_OUT COut
#define TEST_POINT1_IN CIn

#define TEST_POINT1 TEST_POINT1_IN getBit TEST_POINT1_PIN
#define TEST_POINT1_HI TEST_POINT1_OUT setHi TEST_POINT1_PIN
#define TEST_POINT1_LO TEST_POINT1_OUT setLo TEST_POINT1_PIN
#define TEST_POINT1_IS_HI (0 != (TEST_POINT1))
#define TEST_POINT1_IS_LO (0 == (TEST_POINT1))
#define TEST_POINT1_TOGGLE TEST_POINT1_OUT toggleBit TEST_POINT1_PIN

#define TEST_POINT1_SETUP TEST_POINT1_DIR setOut TEST_POINT1_PIN; TEST_POINT1_HI

// test point 2
#define TEST_POINT2_PIN Pin1
#define TEST_POINT2_DIR ADir
#define TEST_POINT2_OUT AOut
#define TEST_POINT2_IN AIn

#define TEST_POINT2 TEST_POINT2_IN getBit TEST_POINT2_PIN
#define TEST_POINT2_HI TEST_POINT2_OUT setHi TEST_POINT2_PIN
#define TEST_POINT2_LO TEST_POINT2_OUT setLo TEST_POINT2_PIN
#define TEST_POINT2_IS_HI (0 != (TEST_POINT2))
#define TEST_POINT2_IS_LO (0 == (TEST_POINT2))
#define TEST_POINT2_TOGGLE TEST_POINT2_OUT toggleBit TEST_POINT2_PIN

#define TEST_POINT2_SETUP TEST_POINT2_DIR setOut TEST_POINT2_PIN; TEST_POINT2_HI

// test point 3
#define TEST_POINT3_PIN Pin5
#define TEST_POINT3_DIR ADir
#define TEST_POINT3_OUT AOut
#define TEST_POINT3_IN AIn

#define TEST_POINT3 TEST_POINT3_IN getBit TEST_POINT3_PIN
#define TEST_POINT3_HI TEST_POINT3_OUT setHi TEST_POINT3_PIN
#define TEST_POINT3_LO TEST_POINT3_OUT setLo TEST_POINT3_PIN
#define TEST_POINT3_IS_HI (0 != (TEST_POINT3))
#define TEST_POINT3_IS_LO (0 == (TEST_POINT3))
#define TEST_POINT3_TOGGLE TEST_POINT3_OUT toggleBit TEST_POINT3_PIN

#define TEST_POINT3_SETUP TEST_POINT3_DIR setOut TEST_POINT3_PIN; TEST_POINT3_HI

/////////////////////// setup ///////////////////////

#define IO_PORTS_SETUP \
NORTH_RX_SETUP; NORTH_TX_SETUP; NORTH_RX_SWITCH_SETUP; \
SOUTH_RX_SETUP; SOUTH_TX_SETUP; SOUTH_RX_SWITCH_SETUP; \
EAST_RX_SETUP; EAST_TX_SETUP; EAST_RX_SWITCH_SETUP; \
LED_ERROR_SETUP; LED_STATUS0_SETUP; LED_STATUS1_SETUP; LED_HEARTBEAT_SETUP; \
TEST_POINT1_SETUP; TEST_POINT2_SETUP; TEST_POINT3_SETUP;

#endif
