/**
 * @author Raoul Rubien
 * 5/2016
 */

#ifndef __TRANSMISSION_INTERRUPTS_H__
#define __TRANSMISSION_INTERRUPTS_H__

#include "Reception.h"

#ifdef SIMULATION

#  include "../../simulation/SimulationMacros.h"

#endif

#  ifdef TRY_INLINE_ISR_RELEVANT
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

/**
 * rectifies/modulates the transmission signal according to the upcoming bit
 */
FUNC_ATTRS void rectifyTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                       void (*txLoImpl)(void)) {
    if (txPort->retainTransmission == false) {
        if (txPort->buffer.pointer.bitMask &
            txPort->buffer.bytes[txPort->buffer.pointer.byteNumber]) {
            txHiImpl();
            IF_SIMULATION_CHAR_OUT('G');
        } else {
            txLoImpl();
            IF_SIMULATION_CHAR_OUT('g');
        }
    }
}

/**
 * modulates the transmission signal according to the current bit and increments the buffer pointer
 */
FUNC_ATTRS void modulateTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                        void (*txLoImpl)(void)) {
    if (txPort->retainTransmission == false) {
        if (isDataEndPosition(txPort)) {
            txLoImpl(); // return signal to default (inverted on receiver side)
            txPort->isTransmitting = false;
            txPort->retainTransmission = true; // stop transmission on empty buffer
        } else {
            // write data bit to output (inverted)
            if (txPort->buffer.pointer.bitMask &
                txPort->buffer.bytes[txPort->buffer.pointer.byteNumber]) {
                txLoImpl();
                IF_SIMULATION_CHAR_OUT('h');
            } else {
                txHiImpl();
                IF_SIMULATION_CHAR_OUT('H');
            }
            bufferBitPointerIncrement(&txPort->buffer.pointer);
        }
    }
    else if (txPort->enableTransmission == true) {
        txPort->isTransmitting = true;
        txPort->enableTransmission = false;
        txPort->retainTransmission = false;
    }
}

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif