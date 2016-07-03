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
    TIMER_TX_RX_COMPARE_VALUE += ParticleAttributes.ports.timerAdjustment.transmissionClockDelayHalf
                                 + ParticleAttributes.ports.timerAdjustment.transmissionClockShift;
}

extern FUNC_ATTRS void scheduleStartTxInterrupt(void);
/**
 * Schedules the next transmission interrupt.
 */
FUNC_ATTRS void scheduleStartTxInterrupt(void) {
    uint16_t counter = TIMER_TX_RX_COUNTER_VALUE;
    TIMER_TX_RX_COMPARE_VALUE = counter -
                                (counter % ParticleAttributes.ports.timerAdjustment.transmissionClockDelay)
                                // TODO: determine adequate factor
                                + 4 * ParticleAttributes.ports.timerAdjustment.transmissionClockDelay
                                + ParticleAttributes.ports.timerAdjustment.transmissionClockShift;
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
