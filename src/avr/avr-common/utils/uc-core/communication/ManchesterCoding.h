/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "ManchesterDecodingTypes.h"

extern FUNC_ATTRS void __rectifyTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                                void (*txLoImpl)(void));
/**
 * rectifies/modulates the transmission signal according to the upcoming bit
 */
FUNC_ATTRS void __rectifyTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                         void (*txLoImpl)(void)) {

    if (isDataEndPosition(txPort)) { // on tx pointer match end position
        txLoImpl(); // return signal to default (inverted at receiver side)
        txPort->isTransmitting = false;
    } else {
        if (txPort->buffer.pointer.bitMask &
            txPort->buffer.bytes[txPort->buffer.pointer.byteNumber]) {
            txHiImpl();
        } else {
            txLoImpl();
        }
    }
}

extern FUNC_ATTRS void __modulateTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                                 void (*txLoImpl)(void));
/**
 * modulates the transmission signal according to the current bit and increments the buffer pointer
 */
FUNC_ATTRS void __modulateTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                          void (*txLoImpl)(void)) {
    if (txPort->buffer.pointer.bitMask &
        txPort->buffer.bytes[txPort->buffer.pointer.byteNumber]) {
        txLoImpl();
    } else {
        txHiImpl();
    }
    bufferBitPointerIncrement(&txPort->buffer.pointer);
}


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

extern FUNC_ATTRS void southTxHiImpl(void);

FUNC_ATTRS void southTxHiImpl(void) {
    SOUTH_TX_HI;
}

extern FUNC_ATTRS void southTxLoImpl(void);

FUNC_ATTRS void southTxLoImpl(void) {
    SOUTH_TX_LO;
}

extern FUNC_ATTRS void eastTxLoImpl(void);

FUNC_ATTRS void eastTxLoImpl(void) {
    EAST_TX_LO;
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

extern FUNC_ATTRS void transmit(volatile TxPort *txPort, void (*txHiImpl)(void), void (*txLoImpl)(void));
/**
 * puts the the next signal on the pin
 */
FUNC_ATTRS void transmit(volatile TxPort *txPort, void (*txHiImpl)(void), void (*txLoImpl)(void)) {
    if (!txPort->isDataBuffered || !txPort->isTransmitting) {
        return;
    }

    if (txPort->isTxClockPhase) {
        __rectifyTransmissionBit(txPort, txHiImpl, txLoImpl);
    } else {
        __modulateTransmissionBit(txPort, txHiImpl, txLoImpl);
    }
    txPort->isTxClockPhase++;
}