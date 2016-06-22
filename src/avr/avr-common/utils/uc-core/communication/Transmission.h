/**
 * @author Raoul Rubien
 * 5/2016
 */

#pragma once

#include "uc-core/interrupts/TimerCounter.h"
#include "simulation/SimulationMacros.h"

extern FUNC_ATTRS void scheduleNextTxInterrupt(void);
/**
 * Schedules the next transmission interrupt.
 */
FUNC_ATTRS void scheduleNextTxInterrupt(void) {
    DEBUG_INT16_OUT(TIMER_TX_RX_COMPARE_VALUE);
    TIMER_TX_RX_COMPARE_VALUE += ParticleAttributes.ports.timerAdjustment.transmissionClockDelayHalf;
    DEBUG_INT16_OUT(TIMER_TX_RX_COMPARE_VALUE);
}

extern FUNC_ATTRS void scheduleStartTxInterrupt(void);
/**
 * Schedules the next transmission interrupt.
 */
FUNC_ATTRS void scheduleStartTxInterrupt(void) {
    uint16_t counter = TIMER_TX_RX_COUNTER_VALUE;
    DEBUG_INT16_OUT(counter);
    TIMER_TX_RX_COMPARE_VALUE = counter - (counter %
                                           ParticleAttributes.ports.timerAdjustment.transmissionClockDelay)
                                + ParticleAttributes.ports.timerAdjustment.transmissionClockDelay;
    DEBUG_INT16_OUT(TIMER_TX_RX_COMPARE_VALUE);
    TIMER_TX_RX_ENABLE_COMPARE_INTERRUPT;
}

extern FUNC_ATTRS void enableTransmission(volatile TxPort *port);
/**
 * Enables the next transmission.
 */
FUNC_ATTRS void enableTransmission(volatile TxPort *port) {
    port->isTxClockPhase = true;
    port->isTransmitting = true;
    scheduleStartTxInterrupt();
}
