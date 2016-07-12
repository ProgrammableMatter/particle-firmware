/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "Communication.h"
#include "ManchesterDecodingTypes.h"
#include "uc-core/configuration/interrupts/TimerCounter.h"
#include "simulation/SimulationMacros.h"
#include "uc-core/configuration/Particle.h"

/**
 * Resets the decoder phase state do default.
 */
#define __resetDecoderPhaseState(decoderPhaseState) \
    (decoderPhaseState = 0)

/**
 * evaluates to true if the phase state indicates a phase at data interval, else to false
 * return values:
 * true     ... indicates a data phase
 * false    ...  indicates a clock phase
 */
#define __isDataPhase(manchesterDecoderPhaseState) \
    (manchesterDecoderPhaseState == 1)

/**
 * updates the phase state by a short interval
 */
#define __phaseStateAdvanceShortInterval(manchesterDecoderPhaseState) \
    (manchesterDecoderPhaseState += 1)

/**
 * updates the phase state by a long interval
 */
#define __phaseStateAdvanceLongInterval(manchesterDecoderPhaseState) \
    (manchesterDecoderPhaseState += 2)

/**
 * updated the cyclce counter by a long interval
 */
#define __cycleCounterAdvanceLongInterval(numberHalfCyclesPassed) \
    (numberHalfCyclesPassed += 2)

/**
 * updated the cyclce counter by a short interval
 */
#define __cycleCounterAdvanceShortInterval(numberHalfCyclesPassed) \
    (numberHalfCyclesPassed += 1)

/**
 * evaluates to a snapshot's uint16_t timer value
 */
#define __getTimerValue(snapshot) \
    (snapshot->timerValue << 1)


extern DECODING_FUNC_ATTRS void __storeDataBit(volatile RxPort *rxPort, const volatile uint8_t isRisingEdge);
/**
 * Stores the data bit to the reception buffer unless the buffer is saturated.
 */
DECODING_FUNC_ATTRS void __storeDataBit(volatile RxPort *rxPort, const volatile uint8_t isRisingEdge) {
    // save bit to buffer
    if (!isBufferEndPosition(&rxPort->buffer.pointer)) {
        if (isRisingEdge) {
//            DEBUG_CHAR_OUT('1');
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] setBit rxPort->buffer.pointer.bitMask;
        } else {
//            DEBUG_CHAR_OUT('0');
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] unsetBit rxPort->buffer.pointer.bitMask;
        }
        bufferBitPointerIncrement(&rxPort->buffer.pointer);
    } else {
        rxPort->isOverflowed = true;
    }
}

extern DECODING_FUNC_ATTRS void __rxSnapshotBufferIncrementStartIndex(volatile RxSnapshotBuffer *o);

DECODING_FUNC_ATTRS void __rxSnapshotBufferIncrementStartIndex(volatile RxSnapshotBuffer *o) {
//    DEBUG_INT16_OUT(o->startIndex);
//    if (o->endIndex >= o->startIndex) {
//        DEBUG_INT16_OUT(o->endIndex - o->startIndex);
//    } else {
//        DEBUG_INT16_OUT(o->endIndex + ((RX_NUMBER_SNAPSHOTS - 1) - o->startIndex + 1));
//    }
    if (o->startIndex >= (RX_NUMBER_SNAPSHOTS - 1)) {
        o->startIndex = 0;
        return;
    }
    o->startIndex++;
}

extern DECODING_FUNC_ATTRS void __rxSnapshotBufferIncrementEndIndex(volatile RxSnapshotBuffer *o);

DECODING_FUNC_ATTRS void __rxSnapshotBufferIncrementEndIndex(volatile RxSnapshotBuffer *o) {

    if (o->endIndex >= (RX_NUMBER_SNAPSHOTS - 1)) {
        o->endIndex = 0;
        return;
    }
    o->endIndex++;

//    DEBUG_INT16_OUT(o->endIndex);
//    if (o->endIndex >= o->startIndex) {
//        DEBUG_INT16_OUT(o->endIndex - o->startIndex);
//    } else {
//        DEBUG_INT16_OUT(o->endIndex + ((RX_NUMBER_SNAPSHOTS - 1) - o->startIndex + 1));
//    }
}

//extern DECODING_FUNC_ATTRS void __rxSnapshotBufferDequeue(volatile RxSnapshotBuffer *o);
/**
 * Releases the 1st element from the the queue.
 */
//DECODING_FUNC_ATTRS void __rxSnapshotBufferDequeue(volatile RxSnapshotBuffer *o) {
//    __rxSnapshotBufferIncrementStartIndex(o);
//}
#define __rxSnapshotBufferDequeue(rxSnapshotBuffer) \
    (__rxSnapshotBufferIncrementStartIndex(rxSnapshotBuffer))

//extern DECODING_FUNC_ATTRS volatile Snapshot *__rxSnapshotBufferPeek(volatile RxSnapshotBuffer *o);
/**
 * Same as dequeue but does not modify the queue.
 */
//DECODING_FUNC_ATTRS volatile Snapshot *__rxSnapshotBufferPeek(volatile RxSnapshotBuffer *o) {
//    return &o->snapshots[o->startIndex];
//}
#define __rxSnapshotBufferPeek(rxSnapshotBuffer) \
    (&(rxSnapshotBuffer)->snapshots[(rxSnapshotBuffer)->startIndex])


//extern DECODING_FUNC_ATTRS bool __rxSnapshotBufferIsEmpty(volatile RxSnapshotBuffer *o);
/**
 * Returns true if the buffer is empty.
 */
//DECODING_FUNC_ATTRS bool __rxSnapshotBufferIsEmpty(volatile RxSnapshotBuffer *o) {
//    return o->startIndex == o->endIndex;
//}
#define __rxSnapshotBufferIsEmpty(rxSnapshotBuffer) \
    ((rxSnapshotBuffer)->startIndex == (rxSnapshotBuffer)->endIndex)

extern DECODING_FUNC_ATTRS void __calculateTimestampLag(volatile uint16_t *a, volatile uint16_t *b,
                                                        volatile uint16_t *result);
/**
 * Calculates the time lag in between two timestamp values.
 */
DECODING_FUNC_ATTRS void __calculateTimestampLag(volatile uint16_t *previousSnapshotValue,
                                                 volatile uint16_t *currentSnapshotValue,
                                                 volatile uint16_t *result) {
    if ((*currentSnapshotValue) >= (*previousSnapshotValue)) {
        *result = *currentSnapshotValue - *previousSnapshotValue;
    } else { // if capture timer counter has overflowed
        *result = (UINT16_MAX - *previousSnapshotValue) + *currentSnapshotValue;
    }
}

extern DECODING_FUNC_ATTRS void captureSnapshot(uint16_t *timerCounterValue, const bool isRisingEdge,
                                                volatile RxSnapshotBuffer *snapshotBuffer);
/**
 * appends the current TIMER_TX_RX_COUNTER value and the specified flank to the snapshot buffer
 */
DECODING_FUNC_ATTRS void captureSnapshot(uint16_t *timerCounterValue, const bool isRisingEdge,
                                         volatile RxSnapshotBuffer *snapshotBuffer) {
    volatile Snapshot *snapshot = &(snapshotBuffer->snapshots[snapshotBuffer->endIndex]);
    __rxSnapshotBufferIncrementEndIndex(snapshotBuffer);
    (*((volatile uint16_t *) (snapshot))) = (*timerCounterValue & 0xFFFE) | isRisingEdge;
}

extern DECODING_FUNC_ATTRS void __approximateNewClockSpeed(volatile RxPort *rxPort);
/**
 * calculates the a new clock speed according to the specified timings
 */
DECODING_FUNC_ATTRS void __approximateNewClockSpeed(volatile RxPort *rxPort) {

    ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable = false;
    __calculateTimestampLag(&rxPort->snapshotsBuffer.temporaryTxStartSnapshotTimerValue,
                            &rxPort->snapshotsBuffer.temporaryTxStopSnapshotTimerValue,
                            &ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay);
    ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay =
            (ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay /
             rxPort->snapshotsBuffer.numberHalfCyclesPassed) * 2;
    ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable = true;
}


extern DECODING_FUNC_ATTRS void __approximateNewClockShift(volatile uint16_t *snapshotValue);
/**
 * calculates the a new clock shift according to the specified timings
 */
DECODING_FUNC_ATTRS void __approximateNewClockShift(volatile uint16_t *snapshotValue) {
    ParticleAttributes.communication.timerAdjustment.isTransmissionClockShiftUpdateable = false;
    ParticleAttributes.communication.timerAdjustment.newTransmissionClockShift =
            (*snapshotValue % ParticleAttributes.communication.timerAdjustment.transmissionClockDelay) / 2;
    ParticleAttributes.communication.timerAdjustment.isTransmissionClockShiftUpdateable = true;
}

extern FUNC_ATTRS void manchesterDecodeBuffer(volatile RxPort *rxPort,
                                              volatile CommunicationProtocolPortState *commPortState,
                                              void (interpreterImpl)
                                                      (volatile RxPort *,
                                                       volatile CommunicationProtocolPortState *));

/**
 * decodes the specified buffer's snapshots to bits and bytes
 */
FUNC_ATTRS void manchesterDecodeBuffer(volatile RxPort *rxPort,
                                       volatile CommunicationProtocolPortState *commPortState,
                                       void (interpreterImpl)
                                               (volatile RxPort *,
                                                volatile CommunicationProtocolPortState *)) {

    if (rxPort->isDataBuffered == true) {
        return;
    }

    switch (rxPort->snapshotsBuffer.decoderStates.decodingState) {

        case DECODER_STATE_TYPE_START:
            if (!__rxSnapshotBufferIsEmpty(&rxPort->snapshotsBuffer)) {
                volatile Snapshot *snapshot = __rxSnapshotBufferPeek(&rxPort->snapshotsBuffer);

                if (snapshot->isRisingEdge == false) {
                    bufferBitPointerStart(&rxPort->buffer.pointer);
                    __resetDecoderPhaseState(rxPort->snapshotsBuffer.decoderStates.phaseState);
                    rxPort->snapshotsBuffer.temporarySnapshotTimerValue = __getTimerValue(snapshot);
                    ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable = false;
                    rxPort->snapshotsBuffer.temporaryTxStartSnapshotTimerValue = rxPort->snapshotsBuffer.temporarySnapshotTimerValue;
                    rxPort->snapshotsBuffer.temporaryTxStopSnapshotTimerValue = rxPort->snapshotsBuffer.temporaryTxStartSnapshotTimerValue;

                    // calculate clock shift for synchronization
                    if (rxPort == &ParticleAttributes.communication.ports.rx.north) {
                        uint16_t timerValue = __getTimerValue(snapshot);
                        __approximateNewClockShift(&timerValue);
                    }

                    DEBUG_CHAR_OUT('|');
                    rxPort->snapshotsBuffer.decoderStates.decodingState = DECODER_STATE_TYPE_DECODING;
                    __rxSnapshotBufferDequeue(&rxPort->snapshotsBuffer);
                    goto __DECODER_STATE_TYPE_DECODING;
                } else {
                    __rxSnapshotBufferDequeue(&rxPort->snapshotsBuffer);
                }
            }
            break;

            // @pre: valid temporary snapshot register
        __DECODER_STATE_TYPE_DECODING:
        case DECODER_STATE_TYPE_DECODING:
            // for all snapshots
            while (!__rxSnapshotBufferIsEmpty(&rxPort->snapshotsBuffer)) {
                volatile Snapshot *snapshot = __rxSnapshotBufferPeek(&rxPort->snapshotsBuffer);
                uint16_t timerValue = __getTimerValue(snapshot);
                uint16_t difference;
                __calculateTimestampLag(&rxPort->snapshotsBuffer.temporarySnapshotTimerValue,
                                        (uint16_t *) &timerValue, &difference);
//                DEBUG_INT16_OUT(difference);

                if (difference <=
                    ParticleAttributes.communication.timerAdjustment.maxLongIntervalDuration) {
                    // on short interval
                    if (difference <=
                        ParticleAttributes.communication.timerAdjustment.maxShortIntervalDuration) {
                        __phaseStateAdvanceShortInterval(rxPort->snapshotsBuffer.decoderStates.phaseState);
                        __cycleCounterAdvanceShortInterval(rxPort->snapshotsBuffer.numberHalfCyclesPassed);
//                        DEBUG_CHAR_OUT('x');
                    }
                        // on long interval
                    else {
                        __phaseStateAdvanceLongInterval(rxPort->snapshotsBuffer.decoderStates.phaseState);
                        __cycleCounterAdvanceLongInterval(rxPort->snapshotsBuffer.numberHalfCyclesPassed);
//                        DEBUG_CHAR_OUT('X');
                    }

                    if (__isDataPhase(rxPort->snapshotsBuffer.decoderStates.phaseState)) {
                        __storeDataBit(rxPort, snapshot->isRisingEdge);
                    }
                    rxPort->snapshotsBuffer.temporarySnapshotTimerValue = __getTimerValue(snapshot);
                    rxPort->snapshotsBuffer.temporaryTxStopSnapshotTimerValue = rxPort->snapshotsBuffer.temporarySnapshotTimerValue;
                    __rxSnapshotBufferDequeue(&rxPort->snapshotsBuffer);
                    // on overdue: difference of two snapshots exceed the max. rx clock duration
                } else {
                    goto __DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS;
                }
            }

            // on empty queue check for timeout
            if (__rxSnapshotBufferIsEmpty(&rxPort->snapshotsBuffer)) {
//                volatile Snapshot *snapshot = __rxSnapshotBufferPeek(&rxPort->snapshotsBuffer);
                uint16_t timerNow = TIMER_TX_RX_COUNTER_VALUE;
                uint16_t difference;
                __calculateTimestampLag(&rxPort->snapshotsBuffer.temporarySnapshotTimerValue, &timerNow,
                                        &difference);
//                DEBUG_INT16_OUT(difference);
                if (difference >=
                    2 * ParticleAttributes.communication.timerAdjustment.transmissionClockDelay) {
                    goto __DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS;
                }
            }
            break;

        __DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS:
        case DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS:
            DEBUG_CHAR_OUT('+');
            __approximateNewClockSpeed(rxPort);
            rxPort->isDataBuffered = true;
            ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable = true;
            rxPort->snapshotsBuffer.decoderStates.decodingState = DECODER_STATE_TYPE_START;
            interpreterImpl(rxPort, commPortState);
            break;
    }
}

