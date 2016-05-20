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

#define __RECEPTION_TIMEOUT_COUNTER_MAX 0xf;

/**
 * Translates the hardware counter to a specific port counter according to the provided TimerCounterAdjustment.
 */
FUNC_ATTRS uint16_t __toPortCounter(const volatile uint16_t *hardwareCounter,
                                    const volatile TimerCounterAdjustment *portTimerArguments) {
    uint16_t portCounter = *hardwareCounter + portTimerArguments->receptionOffset;
    if (portCounter >= TIMER_TX_RX_COMPARE_TOP_VALUE) {
        return portCounter - TIMER_TX_RX_COMPARE_TOP_VALUE;
    } else {
        return portCounter;
    }
}

/**
 * Stores the data bit to the reception buffer unless the buffer is saturated.
 */
FUNC_ATTRS void __storeDataBit(volatile RxPort *rxPort, const volatile uint8_t isRisingEdge) {
    // save bit to buffer
    if (!isRxBufferFull(&(rxPort->buffer.pointer))) {
        if (isRisingEdge) {
            IF_SIMULATION_CHAR_OUT('1');
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] setBit rxPort->buffer.pointer.bitMask;
        } else {
            IF_SIMULATION_CHAR_OUT('0');
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] unsetBit rxPort->buffer.pointer.bitMask;
        }
        rxBufferBitPointerNext(&(rxPort->buffer.pointer));
    } else {
        rxPort->isOverflowed = true;
    }
}

/**
 * In case the counter is greater than the compare value (i.e. compare interrupt was shifted) returns
 * the amount counted over else the timer.
 */
FUNC_ATTRS uint16_t __getTrimmedReceptionCounter(void) {

#ifdef SIMULATION
    if (TIMER_TX_RX_COUNTER > (2 * TIMER_TX_RX_COMPARE_TOP_VALUE)) {
        IF_SIMULATION_CHAR_OUT('R');
        IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
    }
#endif

    if (TIMER_TX_RX_COUNTER > TIMER_TX_RX_COMPARE_TOP_VALUE) {
        return TIMER_TX_RX_COUNTER - TIMER_TX_RX_COMPARE_TOP_VALUE;
    } else {
        return TIMER_TX_RX_COUNTER;
    }
}

/**
 * Handles received data flanks and stores them according to the received time relative to the reception
 * timer/counter.
 */
FUNC_ATTRS void dispatchReceivedDataEdge(volatile RxPort *rxPort,
                                         const bool isRisingEdge) {
    uint16_t hardwareCounter = __getTrimmedReceptionCounter();
    uint16_t captureCounter = __toPortCounter(&hardwareCounter, &(rxPort->adjustment));

//    IF_SIMULATION_INT16_OUT(hardwareCounter);
    IF_SIMULATION_INT16_OUT(captureCounter);

    if (isNotReceiving(rxPort)) { // if reception is timed out this call is the first signal of a transmission
        if (isRisingEdge == false) {
            IF_SIMULATION_CHAR_OUT('S');
            // synchronize the counter offset for this channel
            rxPort->adjustment.receptionOffset = TIMER_TX_RX_COMPARE_TOP_VALUE - hardwareCounter;
        }
    }
    else { // if signal occurs approx. at 1/2 of a package clock
        if ((TX_RX_RECEPTION_CLASSIFICATION_VALUE_LEFT_BORDER <= captureCounter) &&
            (captureCounter <= TX_RX_RECEPTION_CLASSIFICATION_VALUE_RIGHT_BORDER)) {
            IF_SIMULATION_CHAR_OUT('B');
            __storeDataBit(rxPort, isRisingEdge);
        }
        else // if signal occurs approx. at the end/beginning of a package clock
        {
            // re-adjust reception offset
            rxPort->adjustment.receptionOffset = TIMER_TX_RX_COMPARE_TOP_VALUE - hardwareCounter;
            IF_SIMULATION_CHAR_OUT('A');
        }
    }
    rxPort->isReceiving = __RECEPTION_TIMEOUT_COUNTER_MAX;
}

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif