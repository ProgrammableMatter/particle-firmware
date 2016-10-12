/**
 * @author Raoul Rubien 22.09.2016
 *
 * Parity related implementation.
 */

#pragma once

#include "uc-core/communication/CommunicationTypes.h"
#include "common/common.h"
#include "simulation/SimulationMacros.h"

/**
 * Stores the even parity bit of the TxPort's buffer to the same buffer.
 * The port data and data end position must be set up correctly.
 */
void setEvenParityBit(TxPort *const txPort) {
    Package *package = (Package *) txPort->buffer.bytes;
    package->asHeader.parityBit = 0;

    uint8_t evenParity = 0;
    uint8_t bitMask;
    // sum up the 1-bits of all full bytes
    for (int8_t byte = 0; byte < txPort->dataEndPos.byteNumber; byte++) {
        bitMask = 1;
        while (bitMask != 0) {
            if ((txPort->buffer.bytes[byte] getBit bitMask) != 0) {
                evenParity++;
            }
            bitMask <<= 1;
        }
    }

    bitMask = 1;
    // sum up te 1-bits of all subsequent bits
    while ((txPort->dataEndPos.bitMask != 0) && (bitMask != 0)) {
        // on end marker reached
        if ((txPort->dataEndPos.bitMask getBit bitMask) != 0) {
            break;
        }

        if ((txPort->buffer.bytes[txPort->dataEndPos.byteNumber] getBit bitMask) != 0) {
            evenParity++;
        }
        bitMask <<= 1;
    }

    package->asHeader.parityBit = (evenParity getBit 1);
}

/**
 * Tests received buffer for even parity.
 */
bool isEvenParity(const RxPort *const rxPort) {
    // on even parity
    if (rxPort->parityBitCounter == 0) {
        return true;
    }
#ifdef SIMULATION
    DEBUG_CHAR_OUT('9');
#endif
    blinkParityErrorForever(rxPort->parityBitCounter);
    return false;
}