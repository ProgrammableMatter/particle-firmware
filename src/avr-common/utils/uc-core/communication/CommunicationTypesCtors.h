/**
 * @author Raoul Rubien 2016
 *
 * Communication types constructor implementation.
 */
#pragma once

#include "CommunicationTypes.h"
#include "ManchesterDecodingTypesCtors.h"

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructBufferBitPointer(volatile BufferBitPointer *o);

CTOR_ATTRS void constructBufferBitPointer(volatile BufferBitPointer *o) {
    o->byteNumber = 0;
    o->__pad = 0;
    o->bitMask = 1;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructPortBuffer(volatile PortBuffer *o);

CTOR_ATTRS void constructPortBuffer(volatile PortBuffer *o) {
    for (uint8_t i = 0; i < sizeof(o->bytes); i++) {
        o->bytes[i] = 0;
    }
    o->bytes[0] = 0x1; // set start bit
    constructBufferBitPointer(&(o->pointer));
    o->receptionStartTimestamp = 0;
    o->receptionEndTimestamp = 0;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructTxPort(TxPort *o);

CTOR_ATTRS void constructTxPort(TxPort *o) {
    constructPortBuffer(&(o->buffer));
    constructBufferBitPointer(&o->dataEndPos);
    o->isTransmitting = false;
    o->isTxClockPhase = false;
    o->isDataBuffered = false;

    o->isTxClockPhase = false;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructTxPorts(TxPorts *o);

CTOR_ATTRS void constructTxPorts(TxPorts *o) {
    constructTxPort(&o->north);
    constructTxPort(&o->east);
    constructTxPort(&o->south);
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructRxPort(RxPort *o);

CTOR_ATTRS void constructRxPort(RxPort *o) {
    constructRxSnapshotBuffer(&o->snapshotsBuffer);
    constructPortBuffer(&(o->buffer));
    // o->receptionOffset = 0;
    o->isOverflowed = false;
    o->isDataBuffered = false;
    o->parityBitCounter = 0;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructRxPorts(RxPorts *o);

CTOR_ATTRS void constructRxPorts(RxPorts *o) {
    constructRxPort(&(o->north));
    constructRxPort(&(o->east));
    constructRxPort(&(o->south));
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructCommunicationPorts(CommunicationPorts *o);

CTOR_ATTRS void constructCommunicationPorts(CommunicationPorts *o) {
    constructTxPorts(&(o->tx));
    constructRxPorts(&(o->rx));
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructTransmissionTimerAdjustment(TransmissionTimerAdjustment *o);

CTOR_ATTRS void constructTransmissionTimerAdjustment(TransmissionTimerAdjustment *o) {
    o->maxShortIntervalDurationOvertimePercentageRatio = DEFAULT_MAX_SHORT_RECEPTION_OVERTIME_PERCENTAGE_RATIO;
    o->maxShortIntervalDuration =
            (DEFAULT_MAX_SHORT_RECEPTION_OVERTIME_PERCENTAGE_RATIO / 100.0) * DEFAULT_TX_RX_CLOCK_DELAY;
    o->maxLongIntervalDurationOvertimePercentageRatio = DEFAULT_MAX_LONG_RECEPTION_OVERTIME_PERCENTAGE_RATIO;
    o->maxLongIntervalDuration =
            (DEFAULT_MAX_LONG_RECEPTION_OVERTIME_PERCENTAGE_RATIO / 100.0) * DEFAULT_TX_RX_CLOCK_DELAY;

    o->transmissionClockDelay = DEFAULT_TX_RX_CLOCK_DELAY;
    o->transmissionClockDelayHalf = DEFAULT_TX_RX_CLOCK_DELAY >> 1;
    o->newTransmissionClockDelay = 0;
    o->isTransmissionClockDelayUpdateable = false;

    o->transmissionClockShift = 0;
    o->newTransmissionClockShift = 0;
    o->isTransmissionClockShiftUpdateable = false;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructCommunication(Communication *o);

CTOR_ATTRS void constructCommunication(Communication *o) {
    constructTransmissionTimerAdjustment(&o->timerAdjustment);
    constructCommunicationPorts(&o->ports);
}
