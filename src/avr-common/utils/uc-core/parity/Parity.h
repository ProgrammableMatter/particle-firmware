/**
 * @author Raoul Rubien 22.09.2016
 *
 * Parity related implementation.
 */

#pragma once

#include "uc-core/communication/CommunicationTypes.h"
#include "common/common.h"


extern PROTOCOL_FUNC_ATTRS void setParityBit(volatile TxPort *txPort);
/**
 * Stores the odd parity bit of the TxPort's buffer to the same buffer.
 * The port data and data end position must be set up correctly.
 */
PROTOCOL_FUNC_ATTRS void setParityBit(volatile TxPort *txPort) {
    Package *package = (Package *) txPort->buffer.bytes;
    package->asHeader.parityBit = 0;

    uint8_t oddParity = 0;
    uint8_t bitMask;
    // sum up the 1-bits of all full bytes
    for (int8_t byte = 0; byte < txPort->dataEndPos.byteNumber; byte++) {
        bitMask = 1;
        while (bitMask != 0) {
            if ((txPort->buffer.bytes[byte] getBit bitMask) != 0) {
                oddParity++;
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
            oddParity++;
        }
        bitMask <<= 1;
    }

    package->asHeader.parityBit = (oddParity getBit 1);
}

extern FUNC_ATTRS bool isParityBitValid(volatile RxPort *rxPort);
/**
 * Tests whether the received parity counter matches the expected package parity.
 */
FUNC_ATTRS bool isParityBitValid(volatile RxPort *rxPort) {
    volatile Package *package = (Package *) rxPort->buffer.bytes;

    // on parity bit set in received package
    if (package->asHeader.parityBit) {
        // exclude the parity bit from the counter incremented during reception
        rxPort->parityBitCounter++;
    }

    if (rxPort->parityBitCounter != package->asHeader.parityBit) {
        blinkParityErrorForever(rxPort->parityBitCounter);
        // blinkParityErrorForever(package->asHeader.parityBit);
        return false;
    }
    return true;
}