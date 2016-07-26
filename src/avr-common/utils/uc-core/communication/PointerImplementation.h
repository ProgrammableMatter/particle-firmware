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
extern FUNC_ATTRS void northTxHiImpl(void);

FUNC_ATTRS void northTxHiImpl(void) {
    NORTH_TX_HI;
}

/**
 * Writes a logic low to the north transmission pin.
 */
extern FUNC_ATTRS void northTxLoImpl(void);

FUNC_ATTRS void northTxLoImpl(void) {
    NORTH_TX_LO;
}

/**
 * Writes a logic high to the east transmission pin.
 */
extern FUNC_ATTRS void eastTxHiImpl(void);

FUNC_ATTRS void eastTxHiImpl(void) {
    EAST_TX_HI;
}

/**
 * Writes a logic low to the east transmission pin.
 */
extern FUNC_ATTRS void eastTxLoImpl(void);

FUNC_ATTRS void eastTxLoImpl(void) {
    EAST_TX_LO;
}

/**
 * Writes a logic high to the south transmission pin.
 */
extern FUNC_ATTRS void southTxHiImpl(void);

FUNC_ATTRS void southTxHiImpl(void) {
    SOUTH_TX_HI;
}

/**
 * Writes a logic low to the south transmission pin.
 */
extern FUNC_ATTRS void southTxLoImpl(void);

FUNC_ATTRS void southTxLoImpl(void) {
    SOUTH_TX_LO;
}

/**
 * Writes a logic high to the east and south transmission pins.
 */
extern FUNC_ATTRS void simultaneousTxHiImpl(void);

FUNC_ATTRS void simultaneousTxHiImpl(void) {
    EAST_TX_HI;
    SOUTH_TX_HI;
}

/**
 * Writes a logic low to east and south transmission pins.
 */
extern FUNC_ATTRS void simultaneousTxLoImpl(void);

FUNC_ATTRS void simultaneousTxLoImpl(void) {
    EAST_TX_LO;
    SOUTH_TX_LO;
}
