/**
 * @author Raoul Rubien 2016
 */

#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <avr/io.h>
#include "../../simulation/SimulationMacros.h"
#include "CommunicationTypes.h"
#include "../../common/common.h"

#  define FUNC_ATTRS // inline
/**
 * Translates the hardware counter to a specific port counter according to the provided TimerCounterAdjustment.
 */
FUNC_ATTRS uint16_t __toPortCounter(volatile const uint16_t *hardwareCounter,
                                    volatile TimerCounterAdjustment *portTimerArguments) {
    return *hardwareCounter + portTimerArguments->receptionOffset;
}

/**
 * returns true if the captureCounter is within the center interval of the given port arguments
 */
FUNC_ATTRS bool __isInCenterInterval(volatile const uint16_t *hardwareCounter,
                                     volatile TimerCounterAdjustment *portTimerArguments) {
    uint16_t captureCounter = __toPortCounter(hardwareCounter, portTimerArguments);

    IF_SIMULATION_INT16_OUT(*hardwareCounter);
    IF_SIMULATION_CHAR_OUT('b');
    IF_SIMULATION_INT16_OUT(portTimerArguments->estimatedCounterCenter - portTimerArguments->receptionDelta);
    IF_SIMULATION_INT16_OUT(portTimerArguments->estimatedCounterCenter + portTimerArguments->receptionDelta);

    if (
            ((portTimerArguments->estimatedCounterCenter - portTimerArguments->receptionDelta) <=
             captureCounter)
            &&
            (captureCounter <=
             (portTimerArguments->estimatedCounterCenter + portTimerArguments->receptionDelta))
            ) {
        IF_SIMULATION_CHAR_OUT('B');
        return true;
    }
    return false;
}


/**
 * returns true if the captureCounter is within the top interval of the given port arguments
 */
FUNC_ATTRS bool __isInTopInterval(volatile const uint16_t *hardwareCounter,
                                  volatile TimerCounterAdjustment *portTimerArguments) {
    uint16_t captureCounter = __toPortCounter(hardwareCounter, portTimerArguments);
    IF_SIMULATION_CHAR_OUT('a');
    IF_SIMULATION_INT16_OUT(*hardwareCounter);
    if (((portTimerArguments->estimatedCounterTop - portTimerArguments->receptionDelta) <= captureCounter) &&
        (captureCounter <= (portTimerArguments->receptionDelta))) {
        IF_SIMULATION_CHAR_OUT('A');
        return true;
    }
    return false;
}


//static inline void reApproximateTimerCounterMaximumValueOnCenter(const uint16_t *synchronizedCaptureCounter);
/**
 * Adjusts the timer counter maximum value according to the current deviation. The arithmetic average of
 * the current value and the newly estimated one is weighted by a third since the re-approximation depends
 * on three reception channels.
 */
FUNC_ATTRS void __estimateAdjustmentOnCenter(volatile const uint16_t *hardwareCounter,
                                             volatile TimerCounterAdjustment *receptionAdjustment) {
#  ifdef IS_SIMULATION
    uint16_t portTime = __toPortCounter(hardwareCounter, receptionAdjustment);
    uint16_t estimatedTop = 2 * portTime;
    uint16_t estimatedOffset = (estimatedTop - TIMER_TX_RX_COMPARE_TOP_VALUE) / 2.0;
    if (estimatedOffset > UINT8_MAX) {
        IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
    }
    receptionAdjustment->estimatedCounterTop = TIMER_TX_RX_COMPARE_TOP_VALUE + estimatedOffset;
    receptionAdjustment->estimatedTopCounterOffset = estimatedOffset;
#  else
    receptionAdjustment->estimatedCounterTop = (uint8_t) (
            (2 * (*hardwareCounter - receptionAdjustment->receptionOffset) - TIMER_TX_RX_COMPARE_TOP_VALUE) /
            2.0);
    receptionAdjustment->estimatedTopCounterOffset =
            receptionAdjustment->estimatedCounterTop - TIMER_TX_RX_COMPARE_TOP_VALUE;
#  endif
    receptionAdjustment->estimatedCounterCenter =
            receptionAdjustment->estimatedCounterTop / TX_RX_COUNTER_CENTER_VALUE_DIVISOR;
    receptionAdjustment->receptionDelta =
            receptionAdjustment->estimatedCounterTop / TX_RX_RECEPTION_DELTA_VALUE_DIVISOR - 1;
}


//static inline void storeDataBit(volatile RxPort *rxBuffer, uint8_t isRisingEdge);
/**
 * Stores the data bit to the reception buffer unless the buffer is saturated.
 */
FUNC_ATTRS void __storeDataBit(volatile RxPort *rxPort, volatile uint8_t isRisingEdge) {

    if (isBufferFull(&(rxPort->buffer.pointer)) == false) {
        // save bit to buffer
        if (isRisingEdge == 0) {
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] unsetBit rxPort->buffer.pointer.bitMask;
            IF_SIMULATION_CHAR_OUT('0');
        } else {
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] setBit rxPort->buffer.pointer.bitMask;
            IF_SIMULATION_CHAR_OUT('1');
        }
        // increment pointer
        if (rxPort->buffer.pointer.bitMask != (1 << 7)) {
            rxPort->buffer.pointer.bitMask <<= 1;
        } else {
            rxPort->buffer.pointer.bitMask = 1;
            rxPort->buffer.pointer.byteNumber++;
        }
    }
}

/**
 * Handles received data flanks and stores them according to the received time relative to the reception
 * timer/counter.
 */
FUNC_ATTRS void dispatchReceivedDataEdge(volatile RxPort *rxPort, volatile uint8_t isRisingEdge) {
    uint16_t hardwareCounter = TIMER_TX_RX_COUNTER;

    IF_SIMULATION_CHAR_OUT('D');
    // if there is no ongoing reception thus this this call is the first signal of a package
    if (rxPort->isReceiving == 0) {
        IF_SIMULATION_CHAR_OUT('s');
        if (isRisingEdge == 0) {
            IF_SIMULATION_CHAR_OUT('S');
            // synchronize the counter for this channel by using an offset
            // TODO: investigate why ~9 is a good offset value
            rxPort->adjustment.receptionOffset = UINT16_MAX - hardwareCounter;// + 9;
        } else {
        }
    }

    else { // reception is ongoing thus this signal belongs to the current emission
//        // reconstruct the synchronized counter
//        // if signal occurs approx. at 1/2 of a package clock:
//        if (__isInCenterInterval(&hardwareCounter, &(rxPort->adjustment))) {
//            __estimateAdjustmentOnCenter(&hardwareCounter, &(rxPort->adjustment));
//            __storeDataBit(rxPort, isRisingEdge);
//        }
//        else // if signal occurs approx. at the end/beginning of a package clock:
//        if (__isInTopInterval(&hardwareCounter, &(rxPort->adjustment))) {
//            // TODO: re-synchronize channel counter offset
//            // __estimateAdjustmentOnTop(&hardwareCounter, &(rxPort->adjustment));
//
//            // TODO: scale/update DEFAULT_TX_RX_COMPARE_TOP_VALUE in TX_RX_COMPARE_A_VALUE according to all port adjustments
//            // ---- investigate why ~26 is a good offset value
//            // ---- rxPort->adjustment.receptionOffset = TIMER_TX_RX_COUNTER_MAX - hardwareCounter + 26;
//            // TODO: update TIMER_TX_RX_TIMEOUT_COUNTER according to DEFAULT_TX_RX_COMPARE_TOP_VALUE:
//        }
//#  ifdef IS_SIMULATION
//        else {
//            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
//        }
//#  endif
    }

    // data bit separation: 4; safety: 2 => 6
    rxPort->isReceiving = 6;
    TIMER_TX_RX_COUNTER_CLEAR_PENDING_INTERRUPTS;
}


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif