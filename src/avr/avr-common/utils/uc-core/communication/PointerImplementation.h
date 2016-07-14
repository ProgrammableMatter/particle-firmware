/**
 * @author Raoul Rubien 14.07.2016
 */

#pragma once

#include "uc-core/configuration/IoPins.h"

extern FUNC_ATTRS void northTxHiImpl(void);

FUNC_ATTRS void northTxHiImpl(void) {
    NORTH_TX_HI;
}

extern FUNC_ATTRS void northTxLoImpl(void);

FUNC_ATTRS void northTxLoImpl(void) {
    NORTH_TX_LO;
}

extern FUNC_ATTRS void eastTxHiImpl(void);

FUNC_ATTRS void eastTxHiImpl(void) {
    EAST_TX_HI;
}

extern FUNC_ATTRS void eastTxLoImpl(void);

FUNC_ATTRS void eastTxLoImpl(void) {
    EAST_TX_LO;
}

extern FUNC_ATTRS void southTxHiImpl(void);

FUNC_ATTRS void southTxHiImpl(void) {
    SOUTH_TX_HI;
}

extern FUNC_ATTRS void southTxLoImpl(void);

FUNC_ATTRS void southTxLoImpl(void) {
    SOUTH_TX_LO;
}

extern FUNC_ATTRS void simultaneousTxLoImpl(void);

FUNC_ATTRS void simultaneousTxLoImpl(void) {
    EAST_TX_LO;
    SOUTH_TX_LO;
}

extern FUNC_ATTRS void simultaneousTxHiImpl(void);

FUNC_ATTRS void simultaneousTxHiImpl(void) {
    EAST_TX_HI;
    SOUTH_TX_HI;
}
