/**
 * @author Raoul Rubien 2016
 */

#include <avr/pgmspace.h>
#include "../libs/common/common.h"
#include <uc-core/configuration/IoPins.h>

#define SIMULATION
#include <simulation/SimulationMacros.h>

//const char testMessage[] PROGMEM = "test";

/**
 * Generates a certain amount of transitions/writes per output wires/registers. The expected amount of
 * transitions are monitored and evaluated in the simulation by test cases.
 */
int main(void) {
    int extraTransitions = 2;

    extraTransitions++;
    LED_STATUS1_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        LED_STATUS1_OFF;
        LED_STATUS1_ON;
    }

    extraTransitions++;
    LED_STATUS2_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        LED_STATUS2_OFF;
        LED_STATUS2_ON;
    }

    extraTransitions++;
    LED_STATUS3_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        LED_STATUS3_OFF;
        LED_STATUS3_ON;
    }

    extraTransitions++;
    LED_STATUS4_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        LED_STATUS4_OFF;
        LED_STATUS4_ON;
    }

//    extraTransitions++;
//    LED_STATUS5_SETUP;
//    for (int i = 0; i < extraTransitions; ++i) {
//        LED_STATUS5_OFF;
//        LED_STATUS5_ON;
//    }

//    extraTransitions++;
//    LED_STATUS6_SETUP;
//    for (int i = 0; i < extraTransitions; ++i) {
//        LED_STATUS6_OFF;
//        LED_STATUS6_ON;
//    }

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
    TEST_POINT4_SETUP;
    for (int i = 0; i < extraTransitions; ++i) {
        TEST_POINT4_LO;
        TEST_POINT4_HI;
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
        EAST_TX_HI;
        EAST_TX_LO;
    }

    int writes = extraTransitions;
    writes++;
    NORTH_RX_SETUP;
    for (int i = 0; i < writes; ++i) {
        NORTH_RX_PULL_UP_DISABLE;
        NORTH_RX_PULL_UP;
    }

    writes++;
    SOUTH_RX_SETUP;
    for (int i = 0; i < writes; ++i) {
        SOUTH_RX_PULL_UP_DISABLE;
        SOUTH_RX_PULL_UP;
    }

    writes++;
    EAST_RX_SETUP;
    for (int i = 0; i < writes; ++i) {
        EAST_RX_PULL_UP_DISABLE;
        EAST_RX_PULL_UP;
    }

//    writeToUart((PGM_P) pgm_read_word(&(testMessage)));
    writeToUart("test");
    forever { };
}
