/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include <math.h>
#include "Communication.h"
#include "ManchesterDecodingTypes.h"
#include "uc-core/configuration/interrupts/TxRxTimer.h"
#include "simulation/SimulationMacros.h"
#include "uc-core/configuration/Particle.h"
#include "uc-core/configuration/communication/Communication.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/delay/delay.h"
#include "uc-core/periphery/Periphery.h"
#include "uc-core/synchronization/Synchronization.h"

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
 * @param numberHalfCyclesPassed the cycle field to update
 */
#define __cycleCounterAdvanceShortInterval(numberHalfCyclesPassed) \
    (numberHalfCyclesPassed += 1)

/**
 * evaluates to a snapshot's uint16_t timer value
 */
#ifdef MANCHESTER_DECODING_ENABLE_MERGE_TIMESTAMP_WITH_EDGE_DIRECTION
#define __getTimerValue(snapshot) \
    (snapshot->timerValue << 1)
#else
#define __getTimerValue(snapshot) \
    (snapshot->timerValue)
#endif

/**
 * Stores the data bit to the reception buffer unless the buffer is saturated.
 * @param rxPort the port where to buffer the bit
 * @param isRisingEdge the signal edge
 */
static void __storeDataBit(RxPort *const rxPort, const bool isRisingEdge) {
    // save bit to buffer
    if (!isBufferEndPosition(&rxPort->buffer.pointer)) {
        if (isRisingEdge) {
//            DEBUG_CHAR_OUT('1');
//            LED_STATUS2_TOGGLE;
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] setBit rxPort->buffer.pointer.bitMask;
            rxPort->parityBitCounter++;
        } else {
//            DEBUG_CHAR_OUT('0');
//            LED_STATUS3_TOGGLE;
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] unsetBit rxPort->buffer.pointer.bitMask;
        }
        bufferBitPointerIncrement(&rxPort->buffer.pointer);
    } else {
        rxPort->isOverflowed = true;
#ifndef PERIPHERY_REMOVE_IMPL
        // TODO: evaluation code
        uint8_t portNumber = 5;
        if (rxPort == ParticleAttributes.directionOrientedPorts.north.rxPort) {
            portNumber = 1;
        } else if (rxPort == ParticleAttributes.directionOrientedPorts.east.rxPort) {
            portNumber = 2;
        } else if (rxPort == ParticleAttributes.directionOrientedPorts.south.rxPort) {
            portNumber = 3;
        }
        blinkReceptionBufferOverflowErrorForever(&ParticleAttributes.alerts, portNumber);
#endif
    }
}

//#ifdef SIMULATION_doesnotexist
#ifdef SIMULATION

#define __ifSimulationPrintSnapshotBufferSize(rxSnapshotBufferPtr) \
    __printSnapshotBufferSizeToSimulationRegister(rxSnapshotBufferPtr)

/**
 * for evaluation purpose
 */
static void __printSnapshotBufferSizeToSimulationRegister(RxSnapshotBuffer *o) {
    uint16_t size = 0;
    if (o->endIndex > o->startIndex) {
        size = o->endIndex - o->startIndex;
    } else if (o->endIndex < o->startIndex) {
        size = o->endIndex + (MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS - o->startIndex);
    }

    // cut off possible overflows due to race with incomint reception interrupt
    if (size <= MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS) {
        DEBUG_INT16_OUT(size);
    }
}

#else
#  define __ifSimulationPrintSnapshotBufferSize(rxSnapshotBufferPtr)
#endif

/**
 * Increments the snapshots circular buffer start index.
 * @param o the snapshot buffer reference
 */
static void __rxSnapshotBufferIncrementStartIndex(RxSnapshotBuffer *const o) {
    if (o->startIndex >= (MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS - 1)) {
        o->startIndex = 0;
    } else {
        o->startIndex++;
    }

    __ifSimulationPrintSnapshotBufferSize(o);
}

/**
 * Increments the snapshots circular buffer end index.
 * @param o the snapshot buffer reference
 */
static void __rxSnapshotBufferIncrementEndIndex(RxSnapshotBuffer *const o) {
    if (o->endIndex >= (MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS - 1)) {
        o->endIndex = 0;
    } else {
        o->endIndex++;
    }
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
 * @param result the positive difference in between the minuend and subtrahend with respect to
 * timestamp being possibly separated by timer/counter overflow
 */
static void __calculateTimestampLag(const uint16_t *const previousSnapshotValue,
                                    const uint16_t *const currentSnapshotValue,
                                    uint16_t *const result) {
    if (*currentSnapshotValue >= *previousSnapshotValue) {
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
void captureSnapshot(uint16_t *const timerCounterValue, const bool isRisingEdge,
                     RxSnapshotBuffer *const snapshotBuffer) {
    uint8_t nextEndIdx = 0;
    if (snapshotBuffer->endIndex < (MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS - 1)) {
        nextEndIdx = snapshotBuffer->endIndex + 1;
    }

    if (nextEndIdx == snapshotBuffer->startIndex) {
        snapshotBuffer->isOverflowed = true;
#ifdef SIMULATION
        DEBUG_CHAR_OUT('8');
#endif
#ifndef PERIPHERY_REMOVE_IMPL
        // TODO: evaluation code
        uint8_t portNumber = 5;
        if (snapshotBuffer == &ParticleAttributes.directionOrientedPorts.north.rxPort->snapshotsBuffer) {
            portNumber = 1;
        } else if (snapshotBuffer ==
                   &ParticleAttributes.directionOrientedPorts.east.rxPort->snapshotsBuffer) {
            portNumber = 2;
        } else if (snapshotBuffer ==
                   &ParticleAttributes.directionOrientedPorts.south.rxPort->snapshotsBuffer) {
            portNumber = 3;
        }
        blinkReceptionSnapshotBufferOverflowErrorForever(&ParticleAttributes.alerts, portNumber);
#endif
    }

    volatile Snapshot *snapshot = &(snapshotBuffer->snapshots[snapshotBuffer->endIndex]);
    __rxSnapshotBufferIncrementEndIndex(snapshotBuffer);

#ifdef MANCHESTER_DECODING_ENABLE_MERGE_TIMESTAMP_WITH_EDGE_DIRECTION
    if (isRisingEdge) {
        (*((volatile uint16_t *) (snapshot))) = (*timerCounterValue & 0xFFFE) | 1;
    } else {
        (*((volatile uint16_t *) (snapshot))) = (*timerCounterValue & 0xFFFE);
    }
#else
    snapshot->timerValue = *timerCounterValue;
    snapshot->isRisingEdge = isRisingEdge;
#endif
    __ifSimulationPrintSnapshotBufferSize(snapshotBuffer);
}

/**
 * State driven decoding of the specified snapshots buffer. The result is a bit oriented stream.
 * The order is the same as the snapshot buffer's. On package end/timeout calls the interpreter with
 * the respective port as argument.
 * @param port the port to decode from and buffer to
 * @param interpreterImpl a interpreter implementation reference
 */
void manchesterDecodeBuffer(DirectionOrientedPort *const port,
                            void (*const interpreterImpl)(DirectionOrientedPort *)) {
    RxPort *rxPort = port->rxPort;
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

                    DEBUG_CHAR_OUT('+');
                    rxPort->parityBitCounter = 0;
                    rxPort->buffer.receptionDuration = 0;
//                    rxPort->buffer.receptionStartTimestamp = rxPort->snapshotsBuffer.temporarySnapshotTimerValue;
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
                                        &timerValue, &difference);
//                DEBUG_INT16_OUT(difference);

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
                    } else {
                    }
                    rxPort->snapshotsBuffer.temporarySnapshotTimerValue = timerValue;
                    rxPort->buffer.lastFallingToRisingDuration = difference;
                    rxPort->buffer.receptionDuration += difference;
//                    rxPort->buffer.receptionEndTimestamp = timerValue;
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
                uint8_t sreg = SREG;
                MEMORY_BARRIER;
                CLI;
                MEMORY_BARRIER;
                uint16_t timerNow = TIMER_TX_RX_COUNTER_VALUE;
                MEMORY_BARRIER;
                SREG = sreg;
                MEMORY_BARRIER;
                uint16_t difference;
                __calculateTimestampLag(&rxPort->snapshotsBuffer.temporarySnapshotTimerValue, &timerNow,
                                        &difference);
//                DEBUG_INT16_OUT(difference);
                if (difference >=
                    (ParticleAttributes.communication.timerAdjustment.transmissionClockDelay * 2)) {
#ifdef SIMULATION
                    rxPort->snapshotsBuffer.decoderStates.decodingState = DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS;
#endif
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
//            __approximateNewTxClockSpeed(rxPort);
            rxPort->isDataBuffered = true;
            rxPort->snapshotsBuffer.decoderStates.decodingState = DECODER_STATE_TYPE_START;
            interpreterImpl(port);
            break;
    }
}

