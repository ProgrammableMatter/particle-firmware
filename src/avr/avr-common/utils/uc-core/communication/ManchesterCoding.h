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
    if (txPort->buffer.pointer.bitMask &
        txPort->buffer.bytes[txPort->buffer.pointer.byteNumber]) {
        txHiImpl();
        DEBUG_CHAR_OUT('G');
    } else {
        txLoImpl();
        DEBUG_CHAR_OUT('g');
    }
//    }
}

extern FUNC_ATTRS void __modulateTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                                 void (*txLoImpl)(void));
/**
 * modulates the transmission signal according to the current bit and increments the buffer pointer
 */
FUNC_ATTRS void __modulateTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                          void (*txLoImpl)(void)) {
    if (isDataEndPosition(txPort)) {
        txLoImpl(); // return signal to default (inverted on receiver side)
        txPort->isTransmitting = false;
    } else {
        txPort->isTransmitting = true;
        // write data bit to output (inverted)
        if (txPort->buffer.pointer.bitMask &
            txPort->buffer.bytes[txPort->buffer.pointer.byteNumber]) {
            txLoImpl();
            DEBUG_CHAR_OUT('h');
        } else {
            txHiImpl();
            DEBUG_CHAR_OUT('H');
        }
        bufferBitPointerIncrement(&txPort->buffer.pointer);
    }
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


/**
 * puts the the next signal on the pin
 */
FUNC_ATTRS void transmit(volatile TxPort *txPort, void (*txHiImpl)(void), void (*txLoImpl)(void)) {
    if (txPort->isTxClockPhase) {
        __rectifyTransmissionBit(txPort, txHiImpl, txLoImpl);
    } else {
        __modulateTransmissionBit(txPort, txHiImpl, txLoImpl);
    }
    txPort->isTxClockPhase++;
}