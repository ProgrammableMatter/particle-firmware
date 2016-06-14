/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "Communication.h"
#include "ManchesterDecodingTypes.h"
#include "uc-core/interrupts/TimerCounter.h"

#ifdef TRY_INLINE
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif

FUNC_ATTRS void constructSnapshot(volatile Snapshot *o) {
    o->isRisingEdge = false;
    o->timerValue = 0;
}

FUNC_ATTRS void constructManchesterDecoderState(volatile ManchesterDecoderStates *o) {
    o->decodingState = DECODER_STATE_TYPE_START;
}

FUNC_ATTRS void constructRxSnapshotBuffer(volatile RxSnapshotBuffer *o) {
    for (uint8_t idx = 0; idx < (sizeof(o->snapshots) / sizeof(Snapshot)); idx++) {
        constructSnapshot(&o->snapshots[idx]);
    }
    o->startIndex = 0;
    o->endIndex = 0;
    o->temporaryDequeueRegister = 0;
}

///**
// * Translates the absolute captured hardware counter to a relative port specific counter according to the provided TimerCounterAdjustment.
// */
//FUNC_ATTRS uint16_t __toRelativeSnapshotValue(const volatile uint16_t *hardwareCounter,
//                                              const volatile uint16_t *receptionOffset) {
//    uint16_t portCounter = *hardwareCounter + *receptionOffset;
//    if (portCounter >= TIMER_TX_RX_COMPARE_TOP_VALUE) {
//        return portCounter - TIMER_TX_RX_COMPARE_TOP_VALUE;
//    } else {
//        return portCounter;
//    }
//}

///**
// * In case the counter is greater than the compare value (i.e. compare interrupt was shifted) returns
// * the amount counted over else the timer.
// */
//FUNC_ATTRS uint16_t __getTrimmedReceptionCounter(void) {
//
////#ifdef SIMULATION
////    if (TIMER_TX_RX_COUNTER > (2 * TIMER_TX_RX_COMPARE_TOP_VALUE)) {
////        DEBUG_CHAR_OUT('R');
////        IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
////    }
////#endif
//
//    if (TIMER_TX_RX_COUNTER > TIMER_TX_RX_COMPARE_TOP_VALUE) {
//        return TIMER_TX_RX_COUNTER - TIMER_TX_RX_COMPARE_TOP_VALUE;
//    } else {
//        return TIMER_TX_RX_COUNTER;
//    }
//}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif
