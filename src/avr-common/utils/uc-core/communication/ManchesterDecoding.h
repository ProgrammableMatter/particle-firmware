/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "Communication.h"
#include "ManchesterDecodingTypes.h"
#include "uc-core/configuration/interrupts/TxRxTimer.h"
#include "simulation/SimulationMacros.h"
#include "uc-core/configuration/Particle.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/delay/delay.h"
#include "uc-core/periphery/Periphery.h"

/**
 * Resets the decoder phase state do default.
 * @param decoderPhaseState the phase state field to reset
 */
#define __resetDecoderPhaseState(decoderPhaseState) \
    (decoderPhaseState = 0)

/**
 * Evaluates to true if the phase state indicates a phase at data interval, otherwise to false.
 * <br/>true     ... indicates a data phase
 * <br/>false    ...  indicates a clock phase
 * @param manchesterDecoderPhaseState the phase state field to evaluate
 */
#define __isDataPhase(manchesterDecoderPhaseState) \
    (manchesterDecoderPhaseState == 1)

/**
 * Updates the phase state by a short interval.
 * @param manchesterDecoderPhaseState the phase state field to update
 */
#define __phaseStateAdvanceShortInterval(manchesterDecoderPhaseState) \
    (manchesterDecoderPhaseState += 1)

/**
 * Updates the phase state by a long interval.
 * @param manchesterDecoderPhaseState the phase state field to update
 */
#define __phaseStateAdvanceLongInterval(manchesterDecoderPhaseState) \
    (manchesterDecoderPhaseState += 2)

/**
 * Updated/advance the cycle counter by a long interval.
 * @param numberHalfCyclesPassed the cycle field to update
 */
#define __cycleCounterAdvanceLongInterval(numberHalfCyclesPassed) \
    (numberHalfCyclesPassed += 2)

/**
 * Updated the cycle counter by a short interval.
 * @param numberHalfCyclesPassed the dycle field to update
 */
#define __cycleCounterAdvanceShortInterval(numberHalfCyclesPassed) \
    (numberHalfCyclesPassed += 1)

/**
 * evaluates to a snapshot's uint16_t timer value
 */
#define __getTimerValue(snapshot) \
    (snapshot->timerValue << 1)


/**
 * Stores the data bit to the reception buffer unless the buffer is saturated.
 * @param rxPort the port where to buffer the bit
 * @param isRisingEdge the signal edge
 */
extern DECODING_FUNC_ATTRS void __storeDataBit(volatile RxPort *rxPort, const volatile uint8_t isRisingEdge);

DECODING_FUNC_ATTRS void __storeDataBit(volatile RxPort *rxPort, const volatile uint8_t isRisingEdge) {
    // save bit to buffer
    if (!isBufferEndPosition(&rxPort->buffer.pointer)) {
        if (isRisingEdge) {
            // DEBUG_CHAR_OUT('1');
            LED_STATUS2_TOGGLE;
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] setBit rxPort->buffer.pointer.bitMask;
        } else {
            // DEBUG_CHAR_OUT('0');
            LED_STATUS3_TOGGLE;
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] unsetBit rxPort->buffer.pointer.bitMask;
        }
        bufferBitPointerIncrement(&rxPort->buffer.pointer);
    } else {
        rxPort->isOverflowed = true;
    }
}

#ifdef SIMULATION

#define __ifSimulationPrintSnapshotBufferSize(rxSnapshotBufferPtr) \
    __printSnapshotBufferSizeToSimulationRegister(rxSnapshotBufferPtr)

extern inline void __printSnapshotBufferSizeToSimulationRegister(volatile RxSnapshotBuffer *o);

/**
 * for evaluation purpose
 */
inline void __printSnapshotBufferSizeToSimulationRegister(volatile RxSnapshotBuffer *o) {
    if (o->endIndex >= o->startIndex) {
        DEBUG_INT16_OUT(o->endIndex - o->startIndex);
    } else {
        DEBUG_INT16_OUT(o->endIndex + (RX_NUMBER_SNAPSHOTS - o->startIndex));
    }
}

#else
#  define __ifSimulationPrintSnapshotBufferSize(rxSnapshotBufferPtr)
#endif

/**
 * Increments the snapshots circular buffer start index.
 * @param o the snapshot buffer reference
 */
extern DECODING_FUNC_ATTRS void __rxSnapshotBufferIncrementStartIndex(volatile RxSnapshotBuffer *o);

DECODING_FUNC_ATTRS void __rxSnapshotBufferIncrementStartIndex(volatile RxSnapshotBuffer *o) {
    if (o->startIndex >= (RX_NUMBER_SNAPSHOTS - 1)) {
        o->startIndex = 0;
        return;
    }
    o->startIndex++;

    __ifSimulationPrintSnapshotBufferSize(o);
}

/**
 * Increments the snapshots circular buffer end index.
 * @param o the snapshot buffer reference
 */
extern DECODING_FUNC_ATTRS void __rxSnapshotBufferIncrementEndIndex(volatile RxSnapshotBuffer *o);

DECODING_FUNC_ATTRS void __rxSnapshotBufferIncrementEndIndex(volatile RxSnapshotBuffer *o) {

    if (o->endIndex >= (RX_NUMBER_SNAPSHOTS - 1)) {
        o->endIndex = 0;
        return;
    }
    o->endIndex++;
}

/**
 * Releases the next element at the start of the queue.
 */
#define __rxSnapshotBufferDequeue(rxSnapshotBuffer) \
    (__rxSnapshotBufferIncrementStartIndex(rxSnapshotBuffer))

/**
 * Evaluates to the next element at the start of the queue.
 */
#define __rxSnapshotBufferPeek(rxSnapshotBuffer) \
    (&(rxSnapshotBuffer)->snapshots[(rxSnapshotBuffer)->startIndex])


/**
 * Evaluates to true if the buffer is empty, to false otherwise.
 */
#define __rxSnapshotBufferIsEmpty(rxSnapshotBuffer) \
    ((rxSnapshotBuffer)->startIndex == (rxSnapshotBuffer)->endIndex)

/**
 * Calculates the time lag in between two timestamp values.
 * @param a minuend
 * @param b subtrahend
 * @param result the positive difference in between the minuend and subtrahend
 */
extern DECODING_FUNC_ATTRS void __calculateTimestampLag(volatile uint16_t *a, volatile uint16_t *b,
                                                        volatile uint16_t *result);

DECODING_FUNC_ATTRS void __calculateTimestampLag(volatile uint16_t *previousSnapshotValue,
                                                 volatile uint16_t *currentSnapshotValue,
                                                 volatile uint16_t *result) {
    if ((*currentSnapshotValue) >= (*previousSnapshotValue)) {
        *result = *currentSnapshotValue - *previousSnapshotValue;
    } else { // if capture timer counter has overflowed
        *result = (UINT16_MAX - *previousSnapshotValue) + *currentSnapshotValue;
    }
}

/**
 * Appends a value and the specified flank to the snapshot buffer. The least significant counter value
 * bit is discarded and used to store the flank.
 * @param timerCounterValue the counter value to store
 * @param isRisingEdge the signal edge
 * @param snapshotsBuffer a reference to the buffer to store to
 *
 */
extern DECODING_FUNC_ATTRS void captureSnapshot(uint16_t *timerCounterValue, const bool isRisingEdge,
                                                volatile RxSnapshotBuffer *snapshotBuffer);

DECODING_FUNC_ATTRS void captureSnapshot(uint16_t *timerCounterValue, const bool isRisingEdge,
                                         volatile RxSnapshotBuffer *snapshotBuffer) {

    uint8_t nextEndIdx = 0;
    if (snapshotBuffer->endIndex >= (RX_NUMBER_SNAPSHOTS - 1)) {
        nextEndIdx = 0;
    } else {
        nextEndIdx = snapshotBuffer->endIndex + 1;
    }

    if (nextEndIdx == snapshotBuffer->startIndex) {
        snapshotBuffer->isOverflowed = true;
        blinkErrorForever();
    }

    volatile Snapshot *snapshot = &(snapshotBuffer->snapshots[snapshotBuffer->endIndex]);
    __rxSnapshotBufferIncrementEndIndex(snapshotBuffer);
    (*((volatile uint16_t *) (snapshot))) = (*timerCounterValue & 0xFFFE) | isRisingEdge;

    __ifSimulationPrintSnapshotBufferSize(snapshotBuffer);
}

/**
 * Calculates the a new transmission clock speed according to the specified timings. The result is stored
 * to the ParticleAttributes.communication.timerAdjustment fields.
 * @param rxPort the reception port to approximate timings from
 */
extern DECODING_FUNC_ATTRS void __approximateNewTxClockSpeed(volatile RxPort *rxPort);

DECODING_FUNC_ATTRS void __approximateNewTxClockSpeed(volatile RxPort *rxPort) {

    ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable = false;
    __calculateTimestampLag(&rxPort->buffer.receptionStartTimestamp,
                            &rxPort->buffer.receptionEndTimestamp,
                            &ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay);
    ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay =
            (ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay /
             rxPort->snapshotsBuffer.numberHalfCyclesPassed) * 2;
    ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable = true;

    // TODO: clock speed adjustment implementation regarding __approximateNewClockSpeed() outcome is missing
}


/**
 * Calculates the a new transmission clock shift/window according to the specified timings.
 * @param snapshotValue the first snapshot of a transmission
 */
extern DECODING_FUNC_ATTRS void __approximateNewTxClockShift(volatile uint16_t *snapshotValue);

DECODING_FUNC_ATTRS void __approximateNewTxClockShift(volatile uint16_t *snapshotValue) {
    ParticleAttributes.communication.timerAdjustment.isTransmissionClockShiftUpdateable = false;
    ParticleAttributes.communication.timerAdjustment.newTransmissionClockShift =
            (*snapshotValue % ParticleAttributes.communication.timerAdjustment.transmissionClockDelay) / 2;
    ParticleAttributes.communication.timerAdjustment.isTransmissionClockShiftUpdateable = true;

    // TODO: clock synchronization implementation regarding __approximateNewClockShift() outcome is missing
}

/**
 * State driven decoding of the specified snapshots buffer. The result is a bit oriented stream.
 * The order is the same as the snapshot buffer's. On package end/timeout calls the interpreter with
 * the respective port as argument.
 * @param port the port to decode from and buffer to
 * @param interpreterImpl a interpreter implementation reference
 */
extern FUNC_ATTRS void manchesterDecodeBuffer(volatile DirectionOrientedPort *port,
                                              void (*interpreterImpl)(volatile DirectionOrientedPort *));

FUNC_ATTRS void manchesterDecodeBuffer(volatile DirectionOrientedPort *port,
                                       void (*interpreterImpl)(volatile DirectionOrientedPort *)) {
    volatile RxPort *rxPort = port->rxPort;
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
//                    rxPort->snapshotsBuffer.temporaryTxStartSnapshotTimerValue = rxPort->snapshotsBuffer.temporarySnapshotTimerValue;
//                    rxPort->snapshotsBuffer.temporaryTxStopSnapshotTimerValue = rxPort->snapshotsBuffer.temporaryTxStartSnapshotTimerValue;

                    // calculate clock shift for synchronization
                    if (rxPort == &ParticleAttributes.communication.ports.rx.north) {
                        uint16_t timerValue = __getTimerValue(snapshot);
                        __approximateNewTxClockShift(&timerValue);
                    }

                    DEBUG_CHAR_OUT('+');
                    rxPort->buffer.receptionStartTimestamp = rxPort->snapshotsBuffer.temporarySnapshotTimerValue;
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
                // DEBUG_INT16_OUT(difference);

                if (difference <=
                    ParticleAttributes.communication.timerAdjustment.maxLongIntervalDuration) {
                    // on short interval
                    if (difference <=
                        ParticleAttributes.communication.timerAdjustment.maxShortIntervalDuration) {
                        __phaseStateAdvanceShortInterval(rxPort->snapshotsBuffer.decoderStates.phaseState);
                        __cycleCounterAdvanceShortInterval(rxPort->snapshotsBuffer.numberHalfCyclesPassed);
                        // DEBUG_CHAR_OUT('x');
                    }
                        // on long interval
                    else {
                        __phaseStateAdvanceLongInterval(rxPort->snapshotsBuffer.decoderStates.phaseState);
                        __cycleCounterAdvanceLongInterval(rxPort->snapshotsBuffer.numberHalfCyclesPassed);
                        // DEBUG_CHAR_OUT('X');
                    }

                    if (__isDataPhase(rxPort->snapshotsBuffer.decoderStates.phaseState)) {
                        __storeDataBit(rxPort, snapshot->isRisingEdge);
                        LED_STATUS1_TOGGLE;
                    } else {
                    }
                    rxPort->snapshotsBuffer.temporarySnapshotTimerValue = __getTimerValue(snapshot);
//                    rxPort->snapshotsBuffer.temporaryTxStopSnapshotTimerValue = rxPort->snapshotsBuffer.temporarySnapshotTimerValue;
                    __rxSnapshotBufferDequeue(&rxPort->snapshotsBuffer);
                    // on overdue: difference of two snapshots exceed the max. rx clock duration
                } else {
#ifdef SIMULATION
                    rxPort->snapshotsBuffer.decoderStates.decodingState = DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS;
#endif
                    goto __DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS;
                }
            }

            // on empty queue check for timeout
            if (__rxSnapshotBufferIsEmpty(&rxPort->snapshotsBuffer)) {
                // volatile Snapshot *snapshot = __rxSnapshotBufferPeek(&rxPort->snapshotsBuffer);
                uint16_t timerNow = TIMER_TX_RX_COUNTER_VALUE;
                uint16_t difference;
                __calculateTimestampLag(&rxPort->snapshotsBuffer.temporarySnapshotTimerValue, &timerNow,
                                        &difference);
                // DEBUG_INT16_OUT(difference);
                if (difference >=
                    2 * ParticleAttributes.communication.timerAdjustment.transmissionClockDelay) {
#ifdef SIMULATION
                    rxPort->snapshotsBuffer.decoderStates.decodingState = DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS;
#endif
                    rxPort->buffer.receptionEndTimestamp = rxPort->snapshotsBuffer.temporarySnapshotTimerValue;
                    goto __DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS;
                }
            }
            break;

        __DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS:
        case DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS:
            DEBUG_CHAR_OUT('|');
#ifdef SIMULATION
        uint16_t value = rxPort->buffer.pointer.byteNumber;
        DEBUG_INT16_OUT(value);
        value = rxPort->buffer.pointer.bitMask;
        DEBUG_INT16_OUT(rxPort->buffer.pointer.bitMask);
#endif
            __approximateNewTxClockSpeed(rxPort);
            rxPort->isDataBuffered = true;
            ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable = true;
            rxPort->snapshotsBuffer.decoderStates.decodingState = DECODER_STATE_TYPE_START;
            interpreterImpl(port);
            break;
    }
}

