/**
 * @author Raoul Rubien  12.07.2016
 *
 * Received package/command related implementation.
 */

#pragma once

#include "CommunicationProtocolTypes.h"
#include "CommunicationProtocolPackageTypesCtors.h"
#include "uc-core/communication/ManchesterDecodingTypes.h"
#include "uc-core/discovery/Discovery.h"
#include "uc-core/communication/Transmission.h"
#include "uc-core/communication/CommunicationTypesCtors.h"

/**
 * Executes a synchronize local time package.
 * Reads the delivered time adds a correction offset and updates the local time.
 */
extern FUNC_ATTRS void executeSynchronizeLocalTimePackage(
        volatile TimePackage *package);//, volatile RxSnapshotBuffer *snapshotBuffer);

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

    // TODO: calculate and update local time interrupt adjustment Particle.localTime.interruptDelay
    // implementation of local time interrupt delay update is missing

    // TODO: calculate and update communication clock delay ParticleAttributes.communication.timerAdjustment.*
    // implementation of communication speed adjustment is missing
}


/**
 * Executes a set local address package.
 */
extern FUNC_ATTRS void executeSetLocalAddress(volatile EnumerationPackage *package);

FUNC_ATTRS void executeSetLocalAddress(volatile EnumerationPackage *package) {
    ParticleAttributes.node.address.row = package->addressRow0;
    ParticleAttributes.node.address.column = package->addressColumn0;
    ParticleAttributes.protocol.hasNetworkGeometryDiscoveryBreadCrumb = package->hasNetworkGeometryDiscoveryBreadCrumb;
}


/**
 * The rightmost bottommost node annonuces it's address.
 * This node relays the package if it is not the origin (top left), otherwise consumes it.
 */
extern FUNC_ATTRS void executeAnnounceNetworkGeometryPackage(
        volatile AnnounceNetworkGeometryPackage *package);

FUNC_ATTRS void executeAnnounceNetworkGeometryPackage(volatile AnnounceNetworkGeometryPackage *package) {

    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.protocol.networkGeometry.rows = package->rows;
        ParticleAttributes.protocol.networkGeometry.columns = package->columns;
        ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
        ParticleAttributes.node.state = STATE_TYPE_SYNC_NEIGHBOUR;
        setInitiatorStateStart(ParticleAttributes.directionOrientedPorts.simultaneous.protocol);

    } else {
        constructAnnounceNetworkGeometryPackage(package->rows, package->columns);
        setInitiatorStateStart(&ParticleAttributes.protocol.ports.north);
        ParticleAttributes.node.state = STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY;
        ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;
    }
}

/**
 * Copies exactly 9 bytes from source to destination.
 */
extern FUNC_ATTRS void __volatileSram9ByteMemcopy(volatile void *source, volatile void *destination);

FUNC_ATTRS void __volatileSram9ByteMemcopy(volatile void *source, volatile void *destination) {
    ((uint16_t *) destination)[0] = ((uint16_t *) source)[0];
    ((uint16_t *) destination)[1] = ((uint16_t *) source)[1];
    ((uint16_t *) destination)[2] = ((uint16_t *) source)[2];
    ((uint16_t *) destination)[3] = ((uint16_t *) source)[3];
    ((uint8_t *) destination)[8] = ((uint8_t *) source)[8];
}

/**
 * Copies the buffer from the source port to destination port and prepares
 * but does not enable the transmission.
 */
extern FUNC_ATTRS void __relayPackage(volatile Package *source, volatile DirectionOrientedPort *destination,
                                      uint16_t dataEndPointer, StateType endState);

FUNC_ATTRS void __relayPackage(volatile Package *source, volatile DirectionOrientedPort *destination,
                               uint16_t dataEndPointer,
                               StateType endState) {
    clearTransmissionPortBuffer(destination->txPort);
    setInitiatorStateStart(destination->protocol);
    __volatileSram9ByteMemcopy(source, destination->txPort->buffer.bytes);
    setBufferDataEndPointer(destination->txPort->dataEndPos, dataEndPointer);
    ParticleAttributes.node.state = endState;
}


/**
 * Forward/route package and execute a set new network geometry command. The network geometry spans a in a rectangular shape
 * from node address (1,1) to inclusive the address transported by the package. If the current
 * address resides outside the range, the particle relays the package and switches to sleep mode.
 * Otherwise it preserves current state. Forwarding is sipped when in broadcast mode.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 */
extern FUNC_ATTRS void executeSetNetworkGeometryPackage(volatile SetNetworkGeometryPackage *package);

FUNC_ATTRS void executeSetNetworkGeometryPackage(volatile SetNetworkGeometryPackage *package) {

    bool deactivateParticle = false;
    if (ParticleAttributes.node.address.row > package->rows ||
        ParticleAttributes.node.address.column > package->columns) {
        deactivateParticle = true;
        ParticleAttributes.node.state = STATE_TYPE_PREPARE_FOR_SLEEP;
    }

    if (!ParticleAttributes.protocol.isBroadcastEnabled) {
        // on disabled broadcast: relay package
        bool routeToEast = ParticleAttributes.discoveryPulseCounters.east.isConnected;
        bool routeToSouth = ParticleAttributes.discoveryPulseCounters.south.isConnected;

        if (routeToEast && routeToSouth) {
            StateType nextState = (deactivateParticle)
                                  ? STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH_THEN_PREPARE_SLEEP
                                  : STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH;
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.simultaneous,
                           SetNetworkGeometryPackageBufferPointerSize, nextState);
        } else if (routeToEast) {
            StateType nextState = (deactivateParticle) ? STATE_TYPE_SENDING_PACKAGE_TO_EAST_THEN_PREPARE_SLEEP
                                                       : STATE_TYPE_SENDING_PACKAGE_TO_EAST;
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.east,
                           SetNetworkGeometryPackageBufferPointerSize, nextState);
        } else if (routeToSouth) {
            StateType nextState = (deactivateParticle)
                                  ? STATE_TYPE_SENDING_PACKAGE_TO_SOUTH_THEN_PREPARE_SLEEP
                                  : STATE_TYPE_SENDING_PACKAGE_TO_SOUTH;
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.south,
                           SetNetworkGeometryPackageBufferPointerSize, nextState);
        }
    }

    ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;

    // update node type accordingly
    if (ParticleAttributes.node.address.row == package->rows) {
        ParticleAttributes.discoveryPulseCounters.south.isConnected = false;
    }
    if (ParticleAttributes.node.address.column == package->columns) {
        ParticleAttributes.discoveryPulseCounters.east.isConnected = false;
    }
    updateAndDetermineNodeType();
}

/**
 * Infer an actuation command from a heat wires package or a heat wires range package
 * for the the east actuator. This ensures the adjacent node is closing the current loop at
 * the respective actuator.
 */
extern FUNC_ATTRS void __inferEastActuatorCommand(volatile Package *package);

FUNC_ATTRS void __inferEastActuatorCommand(volatile Package *package) {

    if (ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        if (package->asHeader.headerId == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES) {
            volatile HeatWiresPackage *heatWiresPackage = &package->asHeatWiresPackage;
            if (heatWiresPackage->northRight) ParticleAttributes.actuationCommand.actuators.eastLeft = true;
            if (heatWiresPackage->northLeft) ParticleAttributes.actuationCommand.actuators.eastRight = true;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresPackage->startTimeStamp + heatWiresPackage->duration;
            ParticleAttributes.actuationCommand.isScheduled = true;
        } else if (package->asHeader.headerId == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE) {
            volatile HeatWiresRangePackage *heatWiresRangePackage = &package->asHeatWiresRangePackage;
            if (heatWiresRangePackage->northRight) ParticleAttributes.actuationCommand.actuators.eastLeft = true;
            if (heatWiresRangePackage->northLeft) ParticleAttributes.actuationCommand.actuators.eastRight = true;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresRangePackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresRangePackage->startTimeStamp + heatWiresRangePackage->duration;
            ParticleAttributes.actuationCommand.isScheduled = true;
        }
    }
}

/**
 * Infer an actuation command from a heat wires package or a heat wires range package
 * for the the south actuator. This ensures the adjacent node is closing the current loop at
 * the respective actuator.
 */
extern FUNC_ATTRS void __inferSouthActuatorCommand(volatile Package *package);

FUNC_ATTRS void __inferSouthActuatorCommand(volatile Package *package) {
    if (ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        if (package->asHeader.headerId == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES) {
            volatile HeatWiresPackage *heatWiresPackage = &package->asHeatWiresPackage;
            if (heatWiresPackage->northRight) ParticleAttributes.actuationCommand.actuators.southLeft = true;
            if (heatWiresPackage->northLeft) ParticleAttributes.actuationCommand.actuators.southRight = true;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresPackage->startTimeStamp + heatWiresPackage->duration;
            ParticleAttributes.actuationCommand.isScheduled = true;
        } else if (package->asHeader.headerId == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE) {
            volatile HeatWiresRangePackage *heatWiresRangePackage = &package->asHeatWiresRangePackage;
            if (heatWiresRangePackage->northRight) ParticleAttributes.actuationCommand.actuators.southLeft = true;
            if (heatWiresRangePackage->northLeft) ParticleAttributes.actuationCommand.actuators.southRight = true;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresRangePackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresRangePackage->startTimeStamp + heatWiresRangePackage->duration;
            ParticleAttributes.actuationCommand.isScheduled = true;
        }
    }
}

/**
 * Interpret a heat wires or heat wires range package and schedule the command.
 */
extern FUNC_ATTRS void __scheduleHeatWiresCommand(volatile Package *package);

FUNC_ATTRS void __scheduleHeatWiresCommand(volatile Package *package) {
    if (package->asHeader.headerId == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES) {
        volatile HeatWiresPackage *heatWiresPackage = &package->asHeatWiresPackage;
        ParticleAttributes.actuationCommand.actuators.northLeft = heatWiresPackage->northLeft;
        ParticleAttributes.actuationCommand.actuators.northRight = heatWiresPackage->northRight;
        ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
        ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                heatWiresPackage->startTimeStamp +
                ((((uint16_t) heatWiresPackage->durationMsb) << 8) | heatWiresPackage->duration);
        ParticleAttributes.protocol.isBroadcastEnabled = heatWiresPackage->enableBroadcast;
        ParticleAttributes.actuationCommand.isScheduled = true;
    } else if (package->asHeader.headerId == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE) {
        volatile HeatWiresRangePackage *heatWiresRangePackage = &package->asHeatWiresRangePackage;
        ParticleAttributes.actuationCommand.actuators.northLeft = heatWiresRangePackage->northLeft;
        ParticleAttributes.actuationCommand.actuators.northRight = heatWiresRangePackage->northRight;
        ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresRangePackage->startTimeStamp;
        ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                heatWiresRangePackage->startTimeStamp +
                ((((uint16_t) heatWiresRangePackage->durationMsb) << 8) | heatWiresRangePackage->duration);
        ParticleAttributes.protocol.isBroadcastEnabled = heatWiresRangePackage->enableBroadcast;
        ParticleAttributes.actuationCommand.isScheduled = true;
    }
}

/**
 * Forward/route package and execute a heat wires package.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 */
extern FUNC_ATTRS void executeHeatWiresPackage(volatile HeatWiresPackage *heatWiresPackage);

FUNC_ATTRS void executeHeatWiresPackage(volatile HeatWiresPackage *heatWiresPackage) {
    if (ParticleAttributes.node.address.row == heatWiresPackage->addressRow0 &&
        ParticleAttributes.node.address.column == heatWiresPackage->addressColumn0 &&
        ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        // on package reached destination: consume package
        __scheduleHeatWiresCommand((Package *) heatWiresPackage);
        return;
    }

    // on package forwarding
    bool routeToEast = false, routeToSouth = false, inferLocalCommand = false;

    if (ParticleAttributes.node.address.column < heatWiresPackage->addressColumn0) {
        routeToEast = true;
    } else if (ParticleAttributes.node.address.column == heatWiresPackage->addressColumn0) {
        routeToSouth = true;
    }

    if ((ParticleAttributes.node.address.row + 1 == heatWiresPackage->addressRow0 &&
         ParticleAttributes.node.address.column == heatWiresPackage->addressColumn0) ||
        (ParticleAttributes.node.address.row == heatWiresPackage->addressRow0 &&
         ParticleAttributes.node.address.column + 1 == heatWiresPackage->addressColumn0)) {
        inferLocalCommand = true;
    }

    if (routeToEast) {
        if (!ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) heatWiresPackage, &ParticleAttributes.directionOrientedPorts.east,
                           HeatWiresPackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST);
            ParticleAttributes.protocol.isBroadcastEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = false;
        }
        if (inferLocalCommand) {
            __inferEastActuatorCommand((Package *) heatWiresPackage);
        }
    } else if (routeToSouth) {
        if (!ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) heatWiresPackage, &ParticleAttributes.directionOrientedPorts.south,
                           HeatWiresPackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
            ParticleAttributes.protocol.isBroadcastEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = false;
        }
        if (inferLocalCommand) {
            __inferSouthActuatorCommand((Package *) heatWiresPackage);
        }
    }
}

/**
 * Forward/route package and execute a heat wires range package.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 */
extern FUNC_ATTRS void executeHeatWiresRangePackage(volatile HeatWiresRangePackage *heatWiresRangePackage);

FUNC_ATTRS void executeHeatWiresRangePackage(volatile HeatWiresRangePackage *heatWiresRangePackage) {
    NodeAddress nodeAddressTopLeft;
    NodeAddress nodeAddressBottomRight;
    nodeAddressTopLeft.row = heatWiresRangePackage->addressRow0;
    nodeAddressTopLeft.column = heatWiresRangePackage->addressColumn0;
    nodeAddressBottomRight.row = heatWiresRangePackage->addressRow1;
    nodeAddressBottomRight.column = heatWiresRangePackage->addressColumn1;

    // route to east if
    // i) current node's address column is less than node range bottom right column
    bool routeToEast = false;
    if (ParticleAttributes.node.address.column < nodeAddressBottomRight.column &&
        ParticleAttributes.directionOrientedPorts.east.discoveryPulseCounter->isConnected) {
        routeToEast = true;
    }

    // route to south if
    // i) current node's column is within node range columns boundary and
    // ii) current node's row is less than node range bottom right row
    bool routeToSouth = false;
    if ((nodeAddressTopLeft.column <= ParticleAttributes.node.address.column &&
         ParticleAttributes.node.address.column <= nodeAddressBottomRight.column) &&
        ParticleAttributes.node.address.row < nodeAddressBottomRight.row &&
        ParticleAttributes.directionOrientedPorts.south.discoveryPulseCounter->isConnected) {
        routeToSouth = true;
    }

    // a) infer local command if
    // i) the current node address is within the node range or
    // ii) belongs to the row above and is within the range columns boundary
    bool inferLocalCommand = false;
    if ((nodeAddressTopLeft.row - 1 <= ParticleAttributes.node.address.row &&
         ParticleAttributes.node.address.row < nodeAddressBottomRight.row) &&
        (nodeAddressTopLeft.column <= ParticleAttributes.node.address.column &&
         ParticleAttributes.node.address.column <= nodeAddressBottomRight.column)) {
        inferLocalCommand = true;
    }

    // b) also infer local command if the range top left node's north port is connected
    // to the parent's east port
    if (nodeAddressTopLeft.row == 1 && ParticleAttributes.node.address.row == 1 &&
        ParticleAttributes.node.address.column == nodeAddressTopLeft.column - 1) {
        inferLocalCommand = true;
    }

    if (routeToEast && routeToSouth) {
        if (!ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) heatWiresRangePackage,
                           &ParticleAttributes.directionOrientedPorts.simultaneous,
                           HeatWiresRangePackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH);
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
        }
        if (inferLocalCommand) {
            __inferEastActuatorCommand((Package *) heatWiresRangePackage);
            __inferSouthActuatorCommand((Package *) heatWiresRangePackage);
        }
    } else if (routeToEast) {
        if (!ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) heatWiresRangePackage, &ParticleAttributes.directionOrientedPorts.east,
                           HeatWiresRangePackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST);
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = false;
        }
        if (inferLocalCommand) {
            __inferEastActuatorCommand((Package *) heatWiresRangePackage);
        }
    } else if (routeToSouth) {
        if (!ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) heatWiresRangePackage,
                           &ParticleAttributes.directionOrientedPorts.south,
                           HeatWiresRangePackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = false;
        }
        if (inferLocalCommand) {
            __inferSouthActuatorCommand((Package *) heatWiresRangePackage);
        }
    }

    // interpret heat wires range command
    if ((nodeAddressTopLeft.row <= ParticleAttributes.node.address.row &&
         ParticleAttributes.node.address.row <= nodeAddressBottomRight.row) &&
        (nodeAddressTopLeft.column <= ParticleAttributes.node.address.column &&
         ParticleAttributes.node.address.column <= nodeAddressBottomRight.column)) {
        __scheduleHeatWiresCommand(((Package *) heatWiresRangePackage));
    }

}

/**
 * Forwards a header package to all connected ports and interpret the relevant content.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 */
extern FUNC_ATTRS void executeHeaderPackage(volatile HeaderPackage *package);

FUNC_ATTRS void executeHeaderPackage(volatile HeaderPackage *package) {

    if (!ParticleAttributes.protocol.isBroadcastEnabled) {
        // on disabled broadcast: relay package
        bool routeToEast = ParticleAttributes.discoveryPulseCounters.east.isConnected;
        bool routeToSouth = ParticleAttributes.discoveryPulseCounters.south.isConnected;

        if (routeToEast && routeToSouth) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.simultaneous,
                           SetNetworkGeometryPackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH);
        } else if (routeToEast) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.east,
                           SetNetworkGeometryPackageBufferPointerSize, STATE_TYPE_SENDING_PACKAGE_TO_EAST);
        } else if (routeToSouth) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.south,
                           SetNetworkGeometryPackageBufferPointerSize, STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
        }
    }

    ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;
}

/**
 * Forwards package and interpret the relevant content.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 */
extern FUNC_ATTRS void executeHeatWiresModePackage(volatile HeatWiresModePackage *package);

FUNC_ATTRS void executeHeatWiresModePackage(volatile HeatWiresModePackage *package) {

    if (!ParticleAttributes.protocol.isBroadcastEnabled) {
        // on disabled broadcast: relay package
        bool routeToEast = ParticleAttributes.discoveryPulseCounters.east.isConnected;
        bool routeToSouth = ParticleAttributes.discoveryPulseCounters.south.isConnected;

        if (routeToEast && routeToSouth) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.simultaneous,
                           SetNetworkGeometryPackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH);
        } else if (routeToEast) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.east,
                           SetNetworkGeometryPackageBufferPointerSize, STATE_TYPE_SENDING_PACKAGE_TO_EAST);
        } else if (routeToSouth) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.south,
                           SetNetworkGeometryPackageBufferPointerSize, STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
        }
    }

    ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;
    ParticleAttributes.actuationCommand.actuationPower.dutyCycleLevel = package->heatMode;
}
