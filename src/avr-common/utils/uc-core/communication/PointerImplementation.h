/**
 * @author Raoul Rubien 14.07.2016
 *
 * Communication related implementation that is ought to be passed as pointer or callback.
 */

#pragma once

#include "uc-core/configuration/IoPins.h"

/**
 * Writes a logic high to the north transmission pin.
 */
void northTxHiImpl(void) {
    NORTH_TX_HI;
}

/**
 * Writes a logic low to the north transmission pin.
 */
void northTxLoImpl(void) {
    NORTH_TX_LO;
}

/**
 * Writes a logic high to the east transmission pin.
 */
void eastTxHiImpl(void) {
    EAST_TX_LO; // must be inverted due to missing MOSFET
}

/**
 * Writes a logic low to the east transmission pin.
 */
void eastTxLoImpl(void) {
    EAST_TX_HI; // must be inverted due to missing MOSFET
}

/**
 * Writes a logic high to the south transmission pin.
 */
void southTxHiImpl(void) {
    SOUTH_TX_HI;
}

/**
 * Writes a logic low to the south transmission pin.
 */
void southTxLoImpl(void) {
    SOUTH_TX_LO;
}

/**
 * Writes a logic high to the east and south transmission pins.
 */
void simultaneousTxHiImpl(void) {
    MEMORY_BARRIER;
    EAST_TX_LO; // must be inverted due to missing MOSFET
    MEMORY_BARRIER;
    SOUTH_TX_HI;
    MEMORY_BARRIER;
}

/**
 * Writes a logic low to east and south transmission pins.
 */
void simultaneousTxLoImpl(void) {
    MEMORY_BARRIER;
    EAST_TX_HI; // must be inverted due to missing MOSFET
    MEMORY_BARRIER;
    SOUTH_TX_LO;
    MEMORY_BARRIER;
}
