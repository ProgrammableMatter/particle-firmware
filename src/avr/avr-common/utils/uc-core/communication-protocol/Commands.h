/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once

#include "CommunicationProtocolTypes.h"
#include "CommunicationProtocolPackageCtors.h"
#include "uc-core/communication/ManchesterDecodingTypes.h"
#include "uc-core/discovery/Discovery.h"


extern FUNC_ATTRS void executeSynchronizeLocalTimePackage(
        volatile TimePackage *package);//, volatile RxSnapshotBuffer *snapshotBufer);
/**
 * prepare local time synchronization
 */
FUNC_ATTRS void executeSynchronizeLocalTimePackage(
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

    // TODO: calculate and update local time interrupt adjustment: Particle.localTime.interruptDelay
    // TODO: calculate and update communication clock delay ParticleAttributes.communication.timerAdjustment.*
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
        ParticleAttributes.node.state = STATE_TYPE_SYNC_NEIGHBOUR;
        setInitiatorStateStart(ParticleAttributes.directionOrientedPorts.simultaneous.protocol);

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
    // update node type accordingly
    if (ParticleAttributes.node.address.row == package->rows) {
        ParticleAttributes.discoveryPulseCounters.south.isConnected = false;
    }
    if (ParticleAttributes.node.address.column == package->columns) {
        ParticleAttributes.discoveryPulseCounters.east.isConnected = false;
    }
    updateAndDetermineNodeType();
}


extern FUNC_ATTRS void executeHeatWiresPackage(volatile HeatWiresPackage *heatWiresPackage);
/**
 * Executes the heat wires package: extracts the payload and schedules the next actuator command.
 */
FUNC_ATTRS void executeHeatWiresPackage(volatile HeatWiresPackage *heatWiresPackage) {

    if (!ParticleAttributes.protocol.isBroadcastEnabled) {
        // TODO: infer local actuation command if package designated for neighbour
        // TODO: route package if currently not in broadcast mode
    }

    if (ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        ParticleAttributes.actuationCommand.actuators.northLeft = heatWiresPackage->northLeft;
        ParticleAttributes.actuationCommand.actuators.northRight = heatWiresPackage->northRight;
        ParticleAttributes.actuationCommand.actuators.eastLeft = heatWiresPackage->eastLeft;
        ParticleAttributes.actuationCommand.actuators.eastRight = heatWiresPackage->eastRight;
        ParticleAttributes.actuationCommand.actuators.southLeft = heatWiresPackage->southLeft;
        ParticleAttributes.actuationCommand.actuators.southRight = heatWiresPackage->southRight;
        ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
        ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                heatWiresPackage->startTimeStamp + heatWiresPackage->duration;
        ParticleAttributes.protocol.isBroadcastEnabled = heatWiresPackage->enableBroadcast;
        ParticleAttributes.actuationCommand.isScheduled = true;
    }
}