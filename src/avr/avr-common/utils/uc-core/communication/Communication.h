/**
 * @author Raoul Rubien 2016
 */

#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <avr/io.h>
#include "../../simulation/SimulationMacros.h"
#include "CommunicationTypes.h"
#include "../../common/common.h"
#include "../ParticleParameters.h"

#  ifdef TRY_INLINE_ISR_RELEVANT
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif
/**
 * Translates the hardware counter to a specific port counter according to the provided TimerCounterAdjustment.
 */
FUNC_ATTRS uint16_t __toPortCounter(const uint16_t *hardwareCounter,
                                    volatile TimerCounterAdjustment *portTimerArguments) {
    uint16_t accumulatedOffset = *hardwareCounter + portTimerArguments->receptionOffset;
    if (accumulatedOffset > TIMER_TX_RX_COMPARE_TOP_VALUE) {
        return accumulatedOffset - TIMER_TX_RX_COMPARE_TOP_VALUE;
    }
    return accumulatedOffset;
}

/**
 * Stores the data bit to the reception buffer unless the buffer is saturated.
 */
FUNC_ATTRS void __storeDataBit(volatile RxPort *rxPort, volatile uint8_t isRisingEdge) {

    if (isBufferFull(&(rxPort->buffer.pointer)) == false) {
        // save bit to buffer
        if (isRisingEdge == 0) {
            IF_SIMULATION_CHAR_OUT('0');
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] unsetBit rxPort->buffer.pointer.bitMask;
        } else {
            IF_SIMULATION_CHAR_OUT('1');
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
 * Resets the counter. In case the interrupt was shifted (less than 1x compare value), the counter is trimmed,
 * else in case of simulation it switches to erroneous state.
 */
FUNC_ATTRS void resetReceptionCounter(void) {
#ifdef SIMULATION
    if (TIMER_TX_RX_COUNTER > (2 * TIMER_TX_RX_COMPARE_TOP_VALUE)) {
        IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
    }
#endif
    if (TIMER_TX_RX_COUNTER > TIMER_TX_RX_COMPARE_TOP_VALUE) {
        TIMER_TX_RX_COUNTER -= TIMER_TX_RX_COMPARE_TOP_VALUE;
    } else {
        TIMER_TX_RX_COUNTER = 0;
    }
}


/**
 * Handles received data flanks and stores them according to the received time relative to the reception
 * timer/counter.
 */
FUNC_ATTRS void dispatchReceivedDataEdge(volatile RxPort *rxPort, volatile uint16_t *receptionDelta,
                                         uint8_t isRisingEdge) {
    uint16_t hardwareCounter = TIMER_TX_RX_COUNTER; //__getTrimmedCounter();
    IF_SIMULATION_INT16_OUT(hardwareCounter);

    // if there is no ongoing reception thus this this call is the first signal of a package
    if (rxPort->isReceiving == 0) {
        if (isRisingEdge == false) {
            IF_SIMULATION_CHAR_OUT('S');
            // synchronize the counter for this channel by using an offset
            rxPort->adjustment.receptionOffset = TIMER_TX_RX_COMPARE_TOP_VALUE - hardwareCounter; // + 9;
            rxPort->isReceiving = 0b111;
        }
    }
    else { // reception is ongoing thus this signal belongs to the current emission

        // reconstruct the synchronized counter
        uint16_t captureCounter = __toPortCounter(&hardwareCounter, &(rxPort->adjustment));
        // if signal occurs approx. at 1/2 of a package clock:
        if ((rxPort->adjustment.leftOfCenter <= captureCounter) &&
            (captureCounter <= rxPort->adjustment.rightOfCenter)) {
            IF_SIMULATION_CHAR_OUT('B');
            __storeDataBit(rxPort, isRisingEdge);

            rxPort->isReceiving = 0b111;
        }

        else // if signal occurs approx. at the end/beginning of a package clock:
        if (((rxPort->adjustment.leftOfTop <= captureCounter) &&
             (captureCounter <= TIMER_TX_RX_COMPARE_TOP_VALUE)) ||
            (captureCounter <= *receptionDelta)) {
            IF_SIMULATION_CHAR_OUT('A');
            rxPort->isReceiving = 0b111;
        }
#  ifdef IS_SIMULATION
        else {
            IF_SIMULATION_CHAR_OUT('x');
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
        }
#  endif
    }
//    resetReceptionCounter();
//    TIMER_TX_RX_COUNTER_CLEAR_PENDING_INTERRUPTS;
}


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif