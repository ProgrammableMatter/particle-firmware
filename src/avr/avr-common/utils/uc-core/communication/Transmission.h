/**
 * @author Raoul Rubien
 * 5/2016
 */

#pragma once

#include "uc-core/interrupts/TimerCounter.h"

FUNC_ATTRS void scheduleNextTransmissionInterrupt(void) {

    TIMER_TX_RX_COMPARE_VALUE = TIMER_TX_RX_COUNTER %
                                ParticleAttributes.ports.timerAdjustment.transmissionClockDelay
                                + 2 * ParticleAttributes.ports.timerAdjustment.transmissionClockDelay;
    // TODO
    // schedule transmission:
    //  calculate next transmission slot
    //  set compareA value

}

FUNC_ATTRS void enableTransmission(volatile TxPort *port) {
    scheduleNextTransmissionInterrupt();
    port->isTransmitting = true;
    // TODO
}
