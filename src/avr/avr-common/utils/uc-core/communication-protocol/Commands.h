//
// Created by Raoul Rubien on 25.05.16.
//

#pragma once

#include "CommunicationProtocolTypes.h"
#include "CommunicationProtocolPackageCtors.h"


extern FUNC_ATTRS void executeSynchronizeLocalTime(volatile TimePackage *package);
/**
 * prepare local time synchronization
 */
FUNC_ATTRS void executeSynchronizeLocalTime(volatile TimePackage *package) {
//    uint16_t counterValue = TIMER_TX_RX_COUNTER_VALUE;
    TIMER_TX_RX_COUNTER_VALUE = package->time -
                                COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_COMPENSATION_OFFSET *
                                (ParticleAttributes.node.address.row - 1 +
                                 ParticleAttributes.node.address.column - 1);
//    if (package->time > counterValue) {
//        ParticleAttributes.timerCounterAdjustment.counterOffset = package->time - counterValue;
//        ParticleAttributes.timerCounterAdjustment.isPositiveCounterOffset = true;
//    } else {
//        ParticleAttributes.timerCounterAdjustment.counterOffset = counterValue - package->time;
//        ParticleAttributes.timerCounterAdjustment.isPositiveCounterOffset = false;
//    }
//    ParticleAttributes.timerCounterAdjustment.isCounterOffsetValid = true;
    ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;
}


extern FUNC_ATTRS void executeSetLocalAddress(volatile EnumerationPackage *package);
/**
 * Updates the local address.
 */
FUNC_ATTRS void executeSetLocalAddress(volatile EnumerationPackage *package) {
    ParticleAttributes.node.address.row = package->addressRow0;
    ParticleAttributes.node.address.column = package->addressColumn0;
    ParticleAttributes.protocol.hasNetworkGeometryDiscoveryBreadCrumb = package->hasNetworkGeometryDiscoveryBreadCrumb;
}


extern FUNC_ATTRS void executeRelayAnnounceNetworkGeometryPackage(
        volatile AnnounceNetworkGeometryPackage *package);
/**
 * If current node is the origin consume the package otherwise put the particle into relaying package state.
 */
FUNC_ATTRS void executeRelayAnnounceNetworkGeometryPackage(volatile AnnounceNetworkGeometryPackage *package) {

    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.protocol.networkGeometry.rows = package->rows;
        ParticleAttributes.protocol.networkGeometry.columns = package->columns;
        ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;

        //TODO refactoring needed
        ParticleAttributes.node.state = STATE_TYPE_SYNC_NEIGHBOUR;
        setInitiatorStateStart(&ParticleAttributes.protocol.ports.east);

    } else {
        clearTransmissionPortBuffer(&ParticleAttributes.communication.ports.tx.north);
        constructAnnounceNetworkGeometryPackage(package->rows, package->columns);
        setInitiatorStateStart(&ParticleAttributes.protocol.ports.north);
        ParticleAttributes.node.state = STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY;
        ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;
    }
}