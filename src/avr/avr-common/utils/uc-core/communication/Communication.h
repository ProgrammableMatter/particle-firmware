/**
 * @author Raoul Rubien 2016
 */

#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <avr/io.h>
#include "CommunicationTypes.h"
#include "../../common/common.h"

//static inline void reApproximateTimerCounterMaximumValueOnCenter(const uint16_t *synchronizedCaptureCounter);
/**
 * Adjusts the timer counter maximum value according to the current deviation. The arithmetic average of
 * the current value and the newly estimated one is weighted by a third since the re-approximation depends
 * on three reception channels.
 */
inline void __reApproximateTimerCounterMaximumValueOnCenter(const uint16_t *synchronizedCaptureCounter) {
    // TODO: should be (oldTop + newEstimatedTop)/2 instead of (constTop + newEstimatedTop)/2
    TIMER_TX_RX_COUNTER = (DEFAULT_TX_RX_COMPARE_TOP_VALUE +
                           (DEFAULT_TX_RX_COMPARE_TOP_VALUE +
                            (2 * (*synchronizedCaptureCounter - DEFAULT_TX_RX_COUNTER_CENTER_VALUE)) / 3)) /
                          2;
}


//static inline void storeDataBit(volatile RxPort *rxBuffer, uint8_t isRisingEdge);
/**
 * Stores the data bit to the reception buffer unless the buffer is saturated.
 */
inline void __storeDataBit(volatile RxPort *rxPort, uint8_t isRisingEdge) {

    if (isBufferFull(&(rxPort->buffer.pointer)) == false) {
        // save bit to buffer
        if (isRisingEdge == 0) {
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] unsetBit rxPort->buffer.pointer.bitMask;
            UDR = '0';
        } else {
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] setBit rxPort->buffer.pointer.bitMask;
            UDR = '1';
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
inline void dispatchReceivedDataEdge(volatile RxPort *rxPort, uint8_t isRisingEdge) {
    uint16_t hardwareCounter = TIMER_TX_RX_COUNTER;

    UDR = 'D';
    // if there is no ongoing reception thus this this call is the first signal of a package
    if (rxPort->isReceiving == 0) {
        UDR = 's';
        if (isRisingEdge == 0) {
            UDR = 'S';
            // synchronize the counter for this channel by using an offset
            // TODO: investigate why ~9 is a good offset value
            rxPort->receptionSyncOffset = TIMER_TX_RX_COUNTER_MAX - hardwareCounter;// + 9;
        } else {
        }
    }

    else { // reception is ongoing thus this signal belongs to the current emission
        // reconstruct the synchronized counter
        uint16_t synchronizedCaptureCounter = hardwareCounter + rxPort->receptionSyncOffset;
        // if signal occurs approx. at 1/2 of a package clock:
        if ((DEFAULT_TX_RX_COUNTER_LEFT_OF_CENTER_VALUE <= synchronizedCaptureCounter) &&
            (synchronizedCaptureCounter <= DEFAULT_TX_RX_COUNTER_RIGHT_OF_CENTER_VALUE)) {
            UDR = 'B';
            __reApproximateTimerCounterMaximumValueOnCenter(&synchronizedCaptureCounter);
            __storeDataBit(rxPort, isRisingEdge);
        }
            // TODO: this case never occurs
            // it does not classify the capture counter correctly or the timing is wrong
        else // if signal occurs approx. at the end/beginning of a package clock:
        if ((DEFAULT_TX_RX_COUNTER_LEFT_OF_TOP_VALUE <= synchronizedCaptureCounter) ||
            (synchronizedCaptureCounter <= DEFAULT_TX_RX_COUNTER_RIGHT_OF_TOP_VALUE)) {
            UDR = 'A';
            // synchronize channel counter counter
            // TODO: investigate why ~26 is a good offset value
            rxPort->receptionSyncOffset = TIMER_TX_RX_COUNTER_MAX - hardwareCounter + 26;
            // update TX_RX_TIMER_EIGHTH_INTERRUPT_VECT timer according to TIMER_TX_RX_COUNTER1
//            TIMER_TX_RX_SYNC_COUNTER = TIMER_TX_RX_COUNTER / 8;
        } else {
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
        }
    }

    // data bit separation: 4; safety: 2 => 6
    rxPort->isReceiving = 6;
    TIMER_TX_RX_COUNTER_CLEAR_PENDING_INTERRUPTS;
}

#endif