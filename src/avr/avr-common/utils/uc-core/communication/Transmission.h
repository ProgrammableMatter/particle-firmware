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
    TIMER_TX_RX_COMPARE_VALUE += ParticleAttributes.communication.timerAdjustment.transmissionClockDelayHalf
                                 + ParticleAttributes.communication.timerAdjustment.transmissionClockShift;
}

extern FUNC_ATTRS void scheduleStartTxInterrupt(void);
/**
 * Schedules the next transmission interrupt.
 */
FUNC_ATTRS void scheduleStartTxInterrupt(void) {
    uint16_t counter = TIMER_TX_RX_COUNTER_VALUE;
    TIMER_TX_RX_COMPARE_VALUE = counter -
                                (counter %
                                 ParticleAttributes.communication.timerAdjustment.transmissionClockDelay)
                                + 2 * ParticleAttributes.communication.timerAdjustment.transmissionClockDelay
                                + ParticleAttributes.communication.timerAdjustment.transmissionClockShift;
    TIMER_TX_RX_ENABLE_COMPARE_INTERRUPT;
}

extern FUNC_ATTRS void enableTransmission(volatile TxPort *port);
/**
 * Enables the next transmission.
 */
FUNC_ATTRS void enableTransmission(volatile TxPort *port) {
    port->isTxClockPhase = true;
    port->isTransmitting = true;
    port->isDataBuffered = true;
    scheduleStartTxInterrupt();
}
