/**
 * @author Raoul Rubien 2016
 */

#include <avr/pgmspace.h>
#include <uc-core/io-configuration/IoDefinitions.h>
#include <simulation/SimulationMacros.h>
#include "../libs/common/common.h"

const char testMessage[] PROGMEM = "test";

/**
 * Generates a certain amount of transitions/writes per output wires/registers. The expected amount of
 * transitions are monitored and evaluated in the simulation by test cases.
 */
int main(void) {
    int extraTransitions = 2;

    extraTransitions++;
    LED_STATUS0_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        LED_STATUS0_OFF;
        LED_STATUS0_ON;
    }

    extraTransitions++;
    LED_STATUS1_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        LED_STATUS1_OFF;
        LED_STATUS1_ON;
    }

    extraTransitions++;
    LED_HEARTBEAT_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        LED_HEARTBEAT_OFF;
        LED_HEARTBEAT_ON;
    }

    extraTransitions++;
    LED_ERROR_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        LED_ERROR_OFF;
        LED_ERROR_ON;
    }

    extraTransitions++;
    TEST_POINT1_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        TEST_POINT1_LO;
        TEST_POINT1_HI;
    }

    extraTransitions++;
    TEST_POINT2_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        TEST_POINT2_LO;
        TEST_POINT2_HI;
    }

    extraTransitions++;
    TEST_POINT3_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        TEST_POINT3_LO;
        TEST_POINT3_HI;
    }

    extraTransitions++;
    NORTH_TX_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        NORTH_TX_LO;
        NORTH_TX_HI;
    }

    extraTransitions++;
    NORTH_RX_SWITCH_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        NORTH_RX_SWITCH_LO;
        NORTH_RX_SWITCH_HI;
    }

    extraTransitions++;
    SOUTH_TX_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        SOUTH_TX_LO;
        SOUTH_TX_HI;
    }

    extraTransitions++;
    SOUTH_RX_SWITCH_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        SOUTH_RX_SWITCH_LO;
        SOUTH_RX_SWITCH_HI;
    }

    extraTransitions++;
    EAST_TX_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        EAST_TX_LO;
        EAST_TX_HI;
    }

    extraTransitions++;
    EAST_RX_SWITCH_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        EAST_RX_SWITCH_LO;
        EAST_RX_SWITCH_HI;
    }

    int writes = extraTransitions;
    writes++;
    NORTH_RX_SETUP;
    for (int i = 0; i < writes; ++i) {
        NORTH_RX_PULL_DOWN;
        NORTH_RX_PULL_UP;
    }

    writes++;
    SOUTH_RX_SETUP;
    for (int i = 0; i < writes; ++i) {
        SOUTH_RX_PULL_DOWN;
        SOUTH_RX_PULL_UP;
    }

    writes++;
    EAST_RX_SETUP;
    for (int i = 0; i < writes; ++i) {
        EAST_RX_PULL_DOWN;
        EAST_RX_PULL_UP;
    }

    writeToUart((PGM_P) pgm_read_word(&(testMessage)));
    forever { };
}
