/**
 * @author Raoul Rubien 2016
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../../common/common.h"


#include "CommunicationTypes.h"

//static inline void reApproximateTimerCounterMaximumValueOnCenter(const uint16_t *synchronizedCaptureCounter);
/**
 * Adjusts the timer counter maximum value according to the current deviation. The arithmetic average of
 * the current value and the newly estimated one is weighted by a third since the re-approximation depends
 * on three reception channels.
 */
inline void __reApproximateTimerCounterMaximumValueOnCenter(const uint16_t *synchronizedCaptureCounter) {
    // TODO: should be (oldTop + newEstimatedTop)/2 instead of (constTop + newEstimatedTop)/2
    TIMER_TX_RX_COUNTER1 = (DEFAULT_TX_RX_COMPARE_TOP_VALUE +
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
        } else {
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] setBit rxPort->buffer.pointer.bitMask;
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
    uint16_t hardwareCounter = TIMER_TX_RX_COUNTER1;

    // if there is no ongoing reception thus this this call is the first signal of a package
    if (rxPort->isReceiving == 0) {
        if (isRisingEdge == 0) {
            // synchronize the counter for this channel by using an offset
            // TODO: investigate why ~9 is a good offset value
            rxPort->receptionSyncOffset = TIMER_TX_RX_COUNTER1_MAX - hardwareCounter;// + 9;
        } else {
        }
    }

    else { // reception is ongoing thus this signal belongs to the current emission
        // reconstruct the synchronized counter
        uint16_t synchronizedCaptureCounter = hardwareCounter + rxPort->receptionSyncOffset;
        // if signal occurs approx. at 1/2 of a package clock:
        if ((DEFAULT_TX_RX_COUNTER_LEFT_OF_CENTER_VALUE <= synchronizedCaptureCounter) &&
            (synchronizedCaptureCounter <= DEFAULT_TX_RX_COUNTER_RIGHT_OF_CENTER_VALUE)) {
            __reApproximateTimerCounterMaximumValueOnCenter(&synchronizedCaptureCounter);
            __storeDataBit(rxPort, isRisingEdge);
        }
        else // if signal occurs approx. at the end/beginning of a package clock:
        if ((DEFAULT_TX_RX_COUNTER_LEFT_OF_TOP_VALUE <= synchronizedCaptureCounter) ||
            (synchronizedCaptureCounter <= DEFAULT_TX_RX_COUNTER_RIGHT_OF_TOP_VALUE)) {
            // synchronize channel counter counter
            // TODO: investiage why ~26 is a good offset value
            rxPort->receptionSyncOffset = TIMER_TX_RX_COUNTER1_MAX - hardwareCounter + 26;
            // update TX_RX_TIMER_EIGHTH_INTERRUPT_VECT timer according to TIMER_TX_RX_COUNTER1
            TIMER_TX_RX_COUNTER0 = TIMER_TX_RX_COUNTER1 / 8;
        }
    }

    rxPort->isReceiving = 2;
    TIMER_TX_RX_COUNTER1_CLEAR_PENDING_INTERRUPTS;
}

#endif