//
// Created by Raoul Rubien on 25.05.16.
//

#pragma once

#include "CommunicationProtocolTypes.h"


extern FUNC_ATTRS void executeSetLocalTime(volatile TimePackage *package);
/**
 * Updates the local time.
 */
FUNC_ATTRS void executeSetLocalTime(volatile TimePackage *package) {
    // TODO: compensation value missing
    TIMER_TX_RX_COUNTER_VALUE = package->time;
}


extern FUNC_ATTRS void executeSetLocalAddress(volatile EnumerationPackage *package);
/**
 * Updates the local address.
 */
FUNC_ATTRS void executeSetLocalAddress(volatile EnumerationPackage *package) {
    ParticleAttributes.node.address.row = package->addressRow0;
    ParticleAttributes.node.address.column = package->addressColumn0;
}