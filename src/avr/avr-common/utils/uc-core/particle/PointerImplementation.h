/**
 * @author Raoul Rubien  13.07.16.
 */

#pragma once

#include "uc-core/communication/ManchesterDecoding.h"
#include "uc-core/communication-protocol/Interpreter.h"

extern FUNC_ATTRS void receiveNorth(void);
/**
 * north port reception implementation
 */
FUNC_ATTRS void receiveNorth(void) {
    if (ParticleAttributes.discoveryPulseCounters.north.isConnected) {
        manchesterDecodeBuffer(&ParticleAttributes.communication.ports.rx.north,
                               &ParticleAttributes.protocol.ports.north, interpretRxBuffer);
    }
}

extern FUNC_ATTRS void receiveEast(void);
/**
 * east port reception implementation
 */
FUNC_ATTRS void receiveEast(void) {
    if (ParticleAttributes.discoveryPulseCounters.east.isConnected) {
        manchesterDecodeBuffer(&ParticleAttributes.communication.ports.rx.east,
                               &ParticleAttributes.protocol.ports.east, interpretRxBuffer);
    }
}

extern FUNC_ATTRS void receiveSouth(void);
/**
 * south port reception implementation
 */
FUNC_ATTRS void receiveSouth(void) {
    if (ParticleAttributes.discoveryPulseCounters.south.isConnected) {
        manchesterDecodeBuffer(&ParticleAttributes.communication.ports.rx.south,
                               &ParticleAttributes.protocol.ports.south, interpretRxBuffer);
    }
}
