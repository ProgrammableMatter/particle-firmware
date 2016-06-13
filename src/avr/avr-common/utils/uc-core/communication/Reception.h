/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include <avr/io.h>
#include "simulation/SimulationMacros.h"
#include "CommunicationTypes.h"
#include "common/common.h"
#include "uc-core/fw-configuration/ParticleParameters.h"

#ifdef TRY_INLINE_ISR_RELEVANT
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif

//#define __RECEPTION_TIMEOUT_COUNTER_MAX 0xF;




///**
// * increments the timeout counters
// */
//FUNC_ATTRS void advanceReceptionTimeoutCounters(void) {
//
//    if (ParticleAttributes.ports.rx.north.isReceiving == 1) {
//        ParticleAttributes.ports.rx.north.isDataBuffered = true;
//    }
//    ParticleAttributes.ports.rx.north.isReceiving >>= 1;
//
//#ifdef SIMULATION
//    if (ParticleAttributes.ports.rx.north.isReceiving == 0) {
//        DEBUG_CHAR_OUT('U');
//    }
//#endif
//
//    if (ParticleAttributes.ports.rx.east.isReceiving == 1) {
//        ParticleAttributes.ports.rx.east.isDataBuffered = true;
//    }
//    ParticleAttributes.ports.rx.east.isReceiving >>= 1;
//
//
//    if (ParticleAttributes.ports.rx.south.isReceiving == 1) {
//        ParticleAttributes.ports.rx.south.isDataBuffered = true;
//    }
//
//    ParticleAttributes.ports.rx.south.isReceiving >>= 1;
//#ifdef SIMULATION
//    if (ParticleAttributes.ports.rx.south.isReceiving == 0) {
//        DEBUG_CHAR_OUT('V');
//    }
//#endif
//
//}


///**
// * Handles received data flanks and stores them according to the received time relative to the reception
// * timer/counter.
// */
//FUNC_ATTRS void dispatchReceivedDataEdge(volatile RxPort *rxPort,
//                                         const bool isRisingEdge) {
//
//    if (rxPort->isDataBuffered) {
//        rxPort->isOverflowed = true;
//        return;
//    }
//
//    uint16_t hardwareCounter = __getTrimmedReceptionCounter();
//    uint16_t captureCounter = __toPortCounter(&hardwareCounter, &(rxPort->adjustment));
//
////    IF_SIMULATION_INT16_OUT(hardwareCounter);
//    DEBUG_INT16_OUT(captureCounter);
//
//    if (isNotReceiving(rxPort)) { // if reception is timed out this call is the first signal of a transmission
//        if (isRisingEdge == false) {
//            DEBUG_CHAR_OUT('S');
//            // synchronize the counter offset for this channel
//            rxPort->adjustment.receptionOffset = TIMER_TX_RX_COMPARE_TOP_VALUE - hardwareCounter;
//        }
//    }
//    else { // if signal occurs approx. at 1/2 of a package clock
//        if ((TX_RX_RECEPTION_CLASSIFICATION_VALUE_LEFT_BORDER <= captureCounter) &&
//            (captureCounter <= TX_RX_RECEPTION_CLASSIFICATION_VALUE_RIGHT_BORDER)) {
////            DEBUG_CHAR_OUT('B');
//            __storeDataBit(rxPort, isRisingEdge);
//        }
//        else // if signal occurs approx. at the end/beginning of a package clock
//        {
//            // re-adjust reception offset
//            rxPort->adjustment.receptionOffset = TIMER_TX_RX_COMPARE_TOP_VALUE - hardwareCounter;
////            DEBUG_CHAR_OUT('A');
//        }
//    }
//    rxPort->isReceiving = __RECEPTION_TIMEOUT_COUNTER_MAX;
//}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif