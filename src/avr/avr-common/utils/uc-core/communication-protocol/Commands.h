//
// Created by Raoul Rubien on 25.05.16.
//

#pragma once

#include "CommunicationProtocolTypes.h"
#include "CommunicationProtocolPackageCtors.h"
#include "uc-core/communication/ManchesterDecodingTypes.h"


extern FUNC_ATTRS void executeSynchronizeLocalTime(
        volatile TimePackage *package);//, volatile RxSnapshotBuffer *snapshotBufer);
/**
 * prepare local time synchronization
 */
FUNC_ATTRS void executeSynchronizeLocalTime(
        volatile TimePackage *package) {//, volatile RxSnapshotBuffer *snapshotBuffer) {
//    DEBUG_INT16_OUT(snapshotBuffer->temporaryTxStopSnapshotTimerValue - snapshotBuffer->temporaryTxStartSnapshotTimerValue);
//    DEBUG_INT16_OUT(TIMER_TX_RX_COUNTER_VALUE);
    TIMER_TX_RX_COUNTER_VALUE = package->time +
                                COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PER_NODE_INTERRUPT_LAG *
                                (ParticleAttributes.node.address.row - 1 +
                                 ParticleAttributes.node.address.column - 1) +
                                package->packageTransmissionLatency +
                                COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PACKAGE_EXECUTION_LAG +
                                COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_MANUAL_ADJUSTMENT;
//    DEBUG_INT16_OUT(TIMER_TX_RX_COUNTER_VALUE);
//    DEBUG_INT16_OUT(package->time);
//    DEBUG_INT16_OUT(package->packageTransmissionLatency);
    ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;
//    __TIMER1_OVERFLOW_INTERRUPT_ENABLE;
//    snapshotBuffer->temporaryTxStopSnapshotTimerValue = snapshotBuffer->temporaryTxStopSnapshotTimerValue - snapshotBuffer->temporaryTxStartSnapshotTimerValue;
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


extern FUNC_ATTRS void executeAnnounceNetworkGeometryPackage(
        volatile AnnounceNetworkGeometryPackage *package);
/**
 * If current node is the origin consume the package otherwise put the particle into relaying package state.
 */
FUNC_ATTRS void executeAnnounceNetworkGeometryPackage(volatile AnnounceNetworkGeometryPackage *package) {

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


extern FUNC_ATTRS void executeSetNetworkGeometryPackage(volatile SetNetworkGeometryPackage *package);
/**
 * If current address is greater than the requested network geometry switch to sleep mode,
 * otherwise preserve current state.
 *
 * @pre: the particle is set to broadcast mode
 */
FUNC_ATTRS void executeSetNetworkGeometryPackage(volatile SetNetworkGeometryPackage *package) {
    ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;
    if (ParticleAttributes.node.address.row > package->rows ||
        ParticleAttributes.node.address.column > package->columns) {
        ParticleAttributes.node.state = STATE_TYPE_SLEEP_MODE;
    }
    // TODO: update the node type accordingly
}