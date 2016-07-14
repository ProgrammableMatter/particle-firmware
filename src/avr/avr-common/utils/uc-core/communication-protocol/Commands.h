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

extern FUNC_ATTRS void __inferLocalEastHeatWiresCommand(volatile HeatWiresPackage *heatWiresPackage);
/**
 * infer a local actuation command if package's destination is the east neighbour
 */
FUNC_ATTRS void __inferLocalEastHeatWiresCommand(volatile HeatWiresPackage *heatWiresPackage) {
    if (ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        ParticleAttributes.actuationCommand.actuators.eastLeft = heatWiresPackage->northRight;
        ParticleAttributes.actuationCommand.actuators.eastRight = heatWiresPackage->northLeft;
        ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
        ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                heatWiresPackage->startTimeStamp + heatWiresPackage->duration;
        ParticleAttributes.actuationCommand.isScheduled = true;
    }
}


extern FUNC_ATTRS void __inferLocalSouthHeatWiresCommand(volatile HeatWiresPackage *heatWiresPackage);
/**
 * infer a local actuation command if package's destination is the south neighbour
 */
FUNC_ATTRS void __inferLocalSouthHeatWiresCommand(volatile HeatWiresPackage *heatWiresPackage) {
    if (ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        ParticleAttributes.actuationCommand.actuators.southLeft = heatWiresPackage->northRight;
        ParticleAttributes.actuationCommand.actuators.southRight = heatWiresPackage->northLeft;
        ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
        ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                heatWiresPackage->startTimeStamp + heatWiresPackage->duration;
        ParticleAttributes.actuationCommand.isScheduled = true;
    }
}

extern FUNC_ATTRS void __volatileSramMemcopy(volatile void *source, volatile void *destination,
                                             uint8_t numBytes);
/**
 * Copies numBytes from source to destination.
 */
FUNC_ATTRS void __volatileSramMemcopy(volatile void *source, volatile void *destination,
                                      uint8_t numBytes) {
    for (uint8_t idx = 0; idx <= numBytes; idx++) {
        ((uint8_t *) destination)[idx] = ((uint8_t *) source)[idx];
    }
}

extern FUNC_ATTRS void __prepareRelayHeatWiresPackage(volatile HeatWiresPackage *heatWiresPackage,
                                                      volatile DirectionOrientedPort *port,
                                                      StateType endState);
/**
 * duplicates heat wires command package and prepares for transmission start
 */
FUNC_ATTRS void __prepareRelayHeatWiresPackage(volatile HeatWiresPackage *heatWiresPackage,
                                               volatile DirectionOrientedPort *port, StateType endState) {
    clearTransmissionPortBuffer(port->txPort);
    __volatileSramMemcopy(heatWiresPackage, &port->txPort->buffer.bytes,
                          (uint8_t) (HeatWiresPackageBufferPointerSize & 0x00ff));
    setBufferDataEndPointer(port->txPort->dataEndPos, HeatWiresPackageBufferPointerSize);
    setInitiatorStateStart(port->protocol);
    ParticleAttributes.protocol.isBroadcastEnabled = false;
    ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = false;
    ParticleAttributes.node.state = endState;
}


extern FUNC_ATTRS void executeHeatWiresPackage(volatile HeatWiresPackage *heatWiresPackage);
/**
 * Executes the heat wires package: extracts the payload and schedules the next actuator command.
 */
FUNC_ATTRS void executeHeatWiresPackage(volatile HeatWiresPackage *heatWiresPackage) {

    if (ParticleAttributes.node.address.row == heatWiresPackage->addressRow0 &&
        ParticleAttributes.node.address.column == heatWiresPackage->addressColumn0 &&
        ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        // on package reached destination: consume package
        ParticleAttributes.actuationCommand.actuators.northLeft = heatWiresPackage->northLeft;
        ParticleAttributes.actuationCommand.actuators.northRight = heatWiresPackage->northRight;
        ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
        ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                heatWiresPackage->startTimeStamp + heatWiresPackage->duration;
        ParticleAttributes.protocol.isBroadcastEnabled = heatWiresPackage->enableBroadcast;
        ParticleAttributes.actuationCommand.isScheduled = true;
    } else if (!ParticleAttributes.protocol.isBroadcastEnabled) {
        // on package forwarding
        if (ParticleAttributes.node.address.row + 1 == heatWiresPackage->addressRow0 &&
            ParticleAttributes.node.address.column == heatWiresPackage->addressColumn0) {
            // on package destination equals south neighbour:
            // infer local command, forward package
            __inferLocalSouthHeatWiresCommand(heatWiresPackage);
            __prepareRelayHeatWiresPackage(
                    (volatile HeatWiresPackage *) &ParticleAttributes.directionOrientedPorts.north.rxPort->buffer.bytes,
                    &ParticleAttributes.directionOrientedPorts.south, STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);

        } else if (ParticleAttributes.node.address.row == heatWiresPackage->addressRow0 &&
                   ParticleAttributes.node.address.column + 1 == heatWiresPackage->addressColumn0) {
            // on package destination equals east neighbour:
            // infer local command, forward package
            __inferLocalEastHeatWiresCommand(heatWiresPackage);
            __prepareRelayHeatWiresPackage(
                    (volatile HeatWiresPackage *) &ParticleAttributes.directionOrientedPorts.north.rxPort->buffer.bytes,
                    &ParticleAttributes.directionOrientedPorts.east, STATE_TYPE_SENDING_PACKAGE_TO_EAST);
        }
        else if (ParticleAttributes.node.address.column < heatWiresPackage->addressColumn0) {
            // on route package to east: forward package
            __prepareRelayHeatWiresPackage(
                    (volatile HeatWiresPackage *) &ParticleAttributes.directionOrientedPorts.north.rxPort->buffer.bytes,
                    &ParticleAttributes.directionOrientedPorts.east, STATE_TYPE_SENDING_PACKAGE_TO_EAST);
        } else if (ParticleAttributes.node.address.row < heatWiresPackage->addressRow0) {
            // on route package to south: forward package
            __prepareRelayHeatWiresPackage(
                    (volatile HeatWiresPackage *) &ParticleAttributes.directionOrientedPorts.north.rxPort->buffer.bytes,
                    &ParticleAttributes.directionOrientedPorts.south, STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
        }
    }
}