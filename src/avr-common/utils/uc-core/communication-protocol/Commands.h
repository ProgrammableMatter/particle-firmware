/**
 * @author Raoul Rubien  12.07.2016
 *
 * Received package/command related implementation.
 */

#pragma once

#include "uc-core/configuration/communication/Commands.h"
#include "CommunicationProtocolTypes.h"
#include "CommunicationProtocolPackageTypesCtors.h"
#include "uc-core/communication/ManchesterDecodingTypes.h"
#include "uc-core/discovery/Discovery.h"
#include "uc-core/communication/Transmission.h"
#include "uc-core/communication/CommunicationTypesCtors.h"
#include "uc-core/synchronization/Synchronization.h"
#include "uc-core/configuration/interrupts/LocalTime.h"
#include "uc-core/time/Time.h"


/**
 * Executes a synchronize local time package.
 * Reads the delivered time adds a correction offset and updates the local time.
 * @param package the package to interpret and execute
 */


void executeSynchronizeLocalTimePackage(const TimePackage *const package, PortBuffer *const portBuffer) {
    // DEBUG_INT16_OUT(snapshotBuffer->temporaryTxStopSnapshotTimerValue - snapshotBuffer->temporaryTxStartSnapshotTimerValue);
    // DEBUG_INT16_OUT(TIMER_TX_RX_COUNTER_VALUE);

    LED_STATUS2_TOGGLE;

    // ------------------ update local time ---------------------------

#ifndef LOCAL_TIME_IN_PHASE_SHIFTING_ON_LOCAL_TIME_UPDATE
    // consider the optional request to update the local time
    if (false == ParticleAttributes.localTime.isNumTimePeriodsPassedUpdateable) {
        ParticleAttributes.localTime.newNumTimePeriodsPassed = package->timePeriod + 2;
        MEMORY_BARRIER;
        ParticleAttributes.localTime.isNumTimePeriodsPassedUpdateable = package->forceTimePeriodUpdate;
    }
#else
    uint8_t sreg = SREG;
    MEMORY_BARRIER;
    CLI;
    MEMORY_BARRIER;
    const uint16_t now = TIMER_TX_RX_COUNTER_VALUE;
    const uint16_t receptionEndTimestamp = portBuffer->localTimeTrackingTimerCounterValueOnPduReceived;
    const uint16_t nextLocalTimeTriggerAfterReception = portBuffer->nextLocalTimeInterruptOnPduReceived;
    MEMORY_BARRIER;
    SREG = sreg;
    MEMORY_BARRIER;
    const uint16_t preTxLatency =
            ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay * 3;
    ParticleAttributes.timeSynchronization.isNextSyncPackageTimeUpdateRequest = package->forceTimePeriodUpdate;

    // consider local time tracking ISR delay shift on local time update
    if (false == ParticleAttributes.localTime.isNewTimerCounterShiftUpdateable &&
        package->forceTimePeriodUpdate) {

        const uint16_t sepPduEndToTimeIsrDelay = nextLocalTimeTriggerAfterReception - receptionEndTimestamp;

        // remote delay from time remote PDU was constructed until remote local time ISR trigger
        const uint32_t sepConstructUntilIsr = roundf(
                // factor of remote phase
                ((float) package->delayUntilNextTimeTrackingIsr /
                 (float) package->localTimeTrackingPeriodInterruptDelay) *
                // apply to local time unit
                (float) ParticleAttributes.localTime.newTimePeriodInterruptDelay
        );

        // ---------------------- phase shift calculation ----------------------
        /**
         * Delay since the next remote local time tracking interrupt triggers after the PDU was constructed
         * until the next local time tracking interrupt triggers after the pdu was received
         * using locally skewed time units.
         */
        const uint32_t totalShiftSeparation =
                // time from pdu reception until next local time ISR
                +sepPduEndToTimeIsrDelay
                // reception latency
                + portBuffer->receptionDuration
                // remote construction until transmission starts
                + preTxLatency
                // delay until remote time ISR triggers when PDU was constructed in local time units
                - sepConstructUntilIsr;

        int32_t shift = totalShiftSeparation;
        while (shift >= ParticleAttributes.localTime.newTimePeriodInterruptDelay) {
            shift -= ParticleAttributes.localTime.newTimePeriodInterruptDelay;
        }

        // cap the value for the next shift to the maximum step
        uint16_t step;
        if (shift > LOCAL_TIME_IN_PHASE_SHIFTING_MAXIMUM_STEP) {
            step = LOCAL_TIME_IN_PHASE_SHIFTING_MAXIMUM_STEP;
        } else {
            step = shift;
        }

        if (shift < (ParticleAttributes.localTime.newTimePeriodInterruptDelay / 2)) {
            // on short side is left, shift left (shorten the interval)
            shift = -step;
        } else {
            // on short side is right, sift right (extend the interval)
            shift = step;
        }

        // expose calculated phase shift to ISR
        ParticleAttributes.localTime.newTimerCounterShift = shift;
        MEMORY_BARRIER;
        ParticleAttributes.localTime.isNewTimerCounterShiftUpdateable = true;

        // ---------------------- reproduce passed time intervals ----------------------
        // TODO: reading TIMER_TX_RX_COUNTER_VALUE should happen as late as possible
//        const uint16_t now = TIMER_TX_RX_COUNTER_VALUE;
        const uint16_t postRxToInterpeterDelay = now - receptionEndTimestamp;

        /**
         * Total delay since the next remote time ISR triggers when PDU was constructed until
         * now.
         */
        uint32_t totalSeparation =
                // PDU received to interpreter latency
                postRxToInterpeterDelay
                // reception latency
                + portBuffer->receptionDuration
                // remote construction until transmission starts
                + preTxLatency
                // delay until remote time ISR triggers when PDU was constructed in local time units
                - sepConstructUntilIsr;

        uint16_t numTimeIntervals = 2;
        while (totalSeparation >= ParticleAttributes.localTime.newTimePeriodInterruptDelay) {
            totalSeparation -= ParticleAttributes.localTime.newTimePeriodInterruptDelay;
            ++numTimeIntervals;
        }

        // expose calculated number of passed periods to ISR
        ParticleAttributes.localTime.newNumTimePeriodsPassed = package->timePeriod + numTimeIntervals;
        MEMORY_BARRIER;
        ParticleAttributes.localTime.isNumTimePeriodsPassedUpdateable = true;
    }

#endif
    // ------------------ approximate new clock skew ---------------------------
    // calculate observed PDU duration
#ifdef SYNCHRONIZATION_TIME_PACKAGE_DURATION_COUNTING_FIRST_TO_LAST_BIT_EDGE
    const uint32_t sample =
            // remove delay from PDU start until 1st bit
            ((portBuffer->receptionDuration - ((uint32_t) portBuffer->firstFallingToRisingDuration))
             // remove delay from last bit until PDU end
             - ((uint32_t) portBuffer->lastFallingToRisingDuration))
            // shift value down by -UINT16_MAX/2
            - (uint32_t) TIME_SYNCHRONIZATION_SAMPLE_OFFSET;
#else
    // shift value down by -UINT16_MAX/2
    const uint32_t sample = portBuffer->receptionDuration - (uint32_t) TIME_SYNCHRONIZATION_SAMPLE_OFFSET;
#endif

    SampleValueType sampleValue = (SampleValueType) sample;
    samplesFifoBufferAddSample(&sampleValue, &ParticleAttributes.timeSynchronization);
    tryApproximateTimings();

    // ------------------ schedule re-transmission of new time package ---------------------------
    // schedule when the new sync. package is to be forwarded according to the current local time
    ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
    ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
    ParticleAttributes.protocol.isBroadcastEnabled = package->header.enableBroadcast;
}


/**
 * Executes a set local address package.
 * @param package the package to interpret and execute
 */
void executeSetLocalAddress(const EnumerationPackage *const package) {
    ParticleAttributes.node.address.row = package->addressRow;
    ParticleAttributes.node.address.column = package->addressColumn;
    ParticleAttributes.protocol.hasNetworkGeometryDiscoveryBreadCrumb = package->hasNetworkGeometryDiscoveryBreadCrumb;
}


/**
 * The rightmost bottommost node announces it's address.
 * This node relays the package if it is not the origin (top left), otherwise consumes it.
 * @param package the package to interpret and execute
 */
void executeAnnounceNetworkGeometryPackage(const AnnounceNetworkGeometryPackage *const package) {

    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.protocol.networkGeometry.rows = package->rows;
        ParticleAttributes.protocol.networkGeometry.columns = package->columns;
        ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
//        ParticleAttributes.node.state = STATE_TYPE_SYNC_NEIGHBOUR;
        ParticleAttributes.node.state = STATE_TYPE_SYNC_NEIGHBOUR_DONE;
        setInitiatorStateStart(ParticleAttributes.directionOrientedPorts.simultaneous.protocol);
    } else {
        constructAnnounceNetworkGeometryPackage(package->rows, package->columns);
        setInitiatorStateStart(&ParticleAttributes.protocol.ports.north);
        ParticleAttributes.node.state = STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY;
        ParticleAttributes.protocol.isBroadcastEnabled = package->header.enableBroadcast;
    }
}

/**
 * Copies exactly 9 bytes from source to destination.
 * @param source where to read the bytes from
 * @param destination where to store the bytes to
 */
static void __volatileSram9ByteMemcopy(const void *const source,
                                       volatile void *const destination) {
    ((uint16_t *) destination)[0] = ((uint16_t *) source)[0];
    ((uint16_t *) destination)[1] = ((uint16_t *) source)[1];
    ((uint16_t *) destination)[2] = ((uint16_t *) source)[2];
    ((uint16_t *) destination)[3] = ((uint16_t *) source)[3];
    ((uint8_t *) destination)[8] = ((uint8_t *) source)[8];
}

/**
 * Copies the buffer from the source port to destination port and prepares
 * but does not enable the transmission.
 * @param source reference to the package to relay
 * @param destination reference to the destination transmission port
 * @param dataEndPointer the pointer marking the data end on buffer
 * @param endState the node state to switch to
 */
static void __relayPackage(const Package *const source,
                           const DirectionOrientedPort *const destination,
                           uint16_t dataEndPointer,
                           StateType endState) {
    clearTransmissionPortBuffer(destination->txPort);
    setInitiatorStateStart(destination->protocol);
    __volatileSram9ByteMemcopy(source, destination->txPort->buffer.bytes);
    setBufferDataEndPointer(&destination->txPort->dataEndPos, dataEndPointer);
    ParticleAttributes.node.state = endState;

    if (destination == &ParticleAttributes.directionOrientedPorts.simultaneous) {
        ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
    } else {
        ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = false;
    }
}


/**
 * Forward/route package and execute a set new network geometry command.
 * The network geometry spans a in a rectangular shape from node address
 * (1,1) to inclusive the address transported by the package.
 * If the current address resides outside the range, the particle relays
 * the package and switches to sleep mode.
 * Otherwise it preserves current state.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 * @param package the package to interpret and execute
 */
void executeSetNetworkGeometryPackage(const SetNetworkGeometryPackage *const package) {

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
            StateType nextState = (deactivateParticle)
                                  ? STATE_TYPE_SENDING_PACKAGE_TO_EAST_THEN_PREPARE_SLEEP
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

    ParticleAttributes.protocol.isBroadcastEnabled = package->header.enableBroadcast;

    // update node type accordingly
    if (ParticleAttributes.node.address.row == package->rows) {
        ParticleAttributes.discoveryPulseCounters.south.isConnected = false;
    }
    if (ParticleAttributes.node.address.column == package->columns) {
        ParticleAttributes.discoveryPulseCounters.east.isConnected = false;
    }
    updateAndDetermineNodeType();
}


static uint16_t __getHeatWiresDuration(HeatWiresPackage const *const package) {
    return (((uint16_t) package->durationMsb) << 8) | package->durationLsb;
}

static uint16_t __getHeatWiresRangeDuration(HeatWiresRangePackage const *const package) {
    return (((uint16_t) package->durationMsb) << 8) | package->durationLsb;
}

/**
 * Infer an actuation command from a heat wires package or a heat wires range package
 * for the the east actuator.
 * This ensures the adjacent node is closing the current loop at the respective actuator.
 * @param package the package to infer actuator actions from
 */
static void __inferEastActuatorCommand(const Package *const package) {
    if (ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE &&
        package->asHeader.id == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES) {
        if (package->asHeader.isRangeCommand) {
            const HeatWiresRangePackage *const heatWiresRangePackage = &package->asHeatWiresRangePackage;
            if (heatWiresRangePackage->northRight) ParticleAttributes.actuationCommand.actuators.eastLeft = true;
            if (heatWiresRangePackage->northLeft) ParticleAttributes.actuationCommand.actuators.eastRight = true;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresRangePackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresRangePackage->startTimeStamp +
                    __getHeatWiresRangeDuration(heatWiresRangePackage);
            ParticleAttributes.actuationCommand.isScheduled = true;
        }
        else {
            const HeatWiresPackage *const heatWiresPackage = &package->asHeatWiresPackage;
            if (heatWiresPackage->northRight) ParticleAttributes.actuationCommand.actuators.eastLeft = true;
            if (heatWiresPackage->northLeft) ParticleAttributes.actuationCommand.actuators.eastRight = true;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresPackage->startTimeStamp + __getHeatWiresDuration(heatWiresPackage);

            ParticleAttributes.actuationCommand.isScheduled = true;
        }
    }
}

/**
 * Infer an actuation command from a heat wires package or a heat wires range package
 * for the the south actuator. This ensures the adjacent node is closing the current loop at
 * the respective actuator.
 * @param package the package to infer actuator actions from
 */
static void __inferSouthActuatorCommand(const Package *const package) {
    if (package->asHeader.id == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES &&
        ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        if (package->asHeader.isRangeCommand) {
            const HeatWiresRangePackage *const heatWiresRangePackage = &package->asHeatWiresRangePackage;
            if (heatWiresRangePackage->northRight) ParticleAttributes.actuationCommand.actuators.southLeft = true;
            if (heatWiresRangePackage->northLeft) ParticleAttributes.actuationCommand.actuators.southRight = true;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresRangePackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresRangePackage->startTimeStamp +
                    __getHeatWiresRangeDuration(heatWiresRangePackage);
            ParticleAttributes.actuationCommand.isScheduled = true;
        } else {
            const HeatWiresPackage *const heatWiresPackage = &package->asHeatWiresPackage;
            if (heatWiresPackage->northRight) ParticleAttributes.actuationCommand.actuators.southLeft = true;
            if (heatWiresPackage->northLeft) ParticleAttributes.actuationCommand.actuators.southRight = true;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresPackage->startTimeStamp + __getHeatWiresDuration(heatWiresPackage);
            ParticleAttributes.actuationCommand.isScheduled = true;
        }
    }
}

/**
 * Interpret a heat wires or heat wires range package and schedule the command.
 * @param package the package to interpret and execute
 */
static void __scheduleHeatWiresCommand(const Package *const package) {
    if (package->asHeader.id == PACKAGE_HEADER_ID_TYPE_HEAT_WIRES) {
        if (package->asHeader.isRangeCommand) {
            const HeatWiresRangePackage *const heatWiresRangePackage = &package->asHeatWiresRangePackage;
            ParticleAttributes.actuationCommand.actuators.northLeft = heatWiresRangePackage->northLeft;
            ParticleAttributes.actuationCommand.actuators.northRight = heatWiresRangePackage->northRight;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresRangePackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresRangePackage->startTimeStamp +
                    __getHeatWiresRangeDuration(heatWiresRangePackage);
            ParticleAttributes.protocol.isBroadcastEnabled = heatWiresRangePackage->header.enableBroadcast;
            ParticleAttributes.actuationCommand.isScheduled = true;
        } else {
            const HeatWiresPackage *const heatWiresPackage = &package->asHeatWiresPackage;
            ParticleAttributes.actuationCommand.actuators.northLeft = heatWiresPackage->northLeft;
            ParticleAttributes.actuationCommand.actuators.northRight = heatWiresPackage->northRight;
            ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = heatWiresPackage->startTimeStamp;
            ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp =
                    heatWiresPackage->startTimeStamp + __getHeatWiresDuration(heatWiresPackage);
            ParticleAttributes.protocol.isBroadcastEnabled = heatWiresPackage->header.enableBroadcast;
            ParticleAttributes.actuationCommand.isScheduled = true;
        }
    }
}

/**
 * Forward/route package and execute a heat wires package.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 * @param package the package to interpret and execute
 */
void executeHeatWiresPackage(const HeatWiresPackage *const package) {
    if (ParticleAttributes.node.address.row == package->addressRow &&
        ParticleAttributes.node.address.column == package->addressColumn &&
        ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        // on package reached destination: consume package
        __scheduleHeatWiresCommand((Package *) package);
        return;
    }

    bool routeToEast = false, routeToSouth = false, inferLocalCommand = false;
    // on package forwarding
    if (ParticleAttributes.node.address.column < package->addressColumn) {
        routeToEast = true;
    } else if (ParticleAttributes.node.address.column == package->addressColumn) {
        routeToSouth = true;
    }

    if ((ParticleAttributes.node.address.row + 1 == package->addressRow &&
         ParticleAttributes.node.address.column == package->addressColumn) ||
        (ParticleAttributes.node.address.row == package->addressRow &&
         ParticleAttributes.node.address.column + 1 == package->addressColumn)) {
        // on destination equals some subsequent neighbor
        inferLocalCommand = true;
    }

    if (routeToEast) {
        if (false == ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) package,
                           &ParticleAttributes.directionOrientedPorts.east,
                           HeatWiresPackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST);
            ParticleAttributes.protocol.isBroadcastEnabled = false;
        }
        if (inferLocalCommand) {
            __inferEastActuatorCommand((Package *) package);
        }
    } else if (routeToSouth) {
        if (false == ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) package,
                           &ParticleAttributes.directionOrientedPorts.south,
                           HeatWiresPackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
            ParticleAttributes.protocol.isBroadcastEnabled = false;
        }
        if (inferLocalCommand) {
            __inferSouthActuatorCommand((Package *) package);
        }
    }
}

/**
 * Forward/route package and execute a heat wires range package.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 * @param package the package to interpret and execute
 */
void executeHeatWiresRangePackage(const HeatWiresRangePackage *const package) {
    NodeAddress nodeAddressTopLeft;
    NodeAddress nodeAddressBottomRight;
    nodeAddressTopLeft.row = package->addressRow0;
    nodeAddressTopLeft.column = package->addressColumn0;
    nodeAddressBottomRight.row = package->addressRow1;
    nodeAddressBottomRight.column = package->addressColumn1;

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
            __relayPackage((Package *) package,
                           &ParticleAttributes.directionOrientedPorts.simultaneous,
                           HeatWiresRangePackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH);
        }
        if (inferLocalCommand) {
            __inferEastActuatorCommand((Package *) package);
            __inferSouthActuatorCommand((Package *) package);
        }
    } else if (routeToEast) {
        if (!ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.east,
                           HeatWiresRangePackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST);
        }
        if (inferLocalCommand) {
            __inferEastActuatorCommand((Package *) package);
        }
    } else if (routeToSouth) {
        if (!ParticleAttributes.protocol.isBroadcastEnabled) {
            __relayPackage((Package *) package,
                           &ParticleAttributes.directionOrientedPorts.south,
                           HeatWiresRangePackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
        }
        if (inferLocalCommand) {
            __inferSouthActuatorCommand((Package *) package);
        }
    }

    // interpret heat wires range command
    if ((nodeAddressTopLeft.row <= ParticleAttributes.node.address.row &&
         ParticleAttributes.node.address.row <= nodeAddressBottomRight.row) &&
        (nodeAddressTopLeft.column <= ParticleAttributes.node.address.column &&
         ParticleAttributes.node.address.column <= nodeAddressBottomRight.column)) {
        __scheduleHeatWiresCommand(((Package *) package));
    }

}

/**
 * Forwards a header package to all connected ports and interpret the relevant content.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 * @param package the package to interpret and execute
 */
void executeHeaderPackage(const HeaderPackage *const package) {

    if (!ParticleAttributes.protocol.isBroadcastEnabled) {
        // on disabled broadcast: relay package
        bool routeToEast = ParticleAttributes.discoveryPulseCounters.east.isConnected;
        bool routeToSouth = ParticleAttributes.discoveryPulseCounters.south.isConnected;

        if (routeToEast && routeToSouth) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.simultaneous,
                           HeaderPackagePointerSize, STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH);
        } else if (routeToEast) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.east,
                           HeaderPackagePointerSize, STATE_TYPE_SENDING_PACKAGE_TO_EAST);
        } else if (routeToSouth) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.south,
                           HeaderPackagePointerSize, STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
        }
    }

    ParticleAttributes.protocol.isBroadcastEnabled = package->enableBroadcast;
}

/**
 * Forwards package and interpret the relevant content.
 * Forwarding is skipped in broadcast mode.
 * Performs simultaneous transmission on splitting points.
 * @param package the package to interpret and execute
 */
void executeHeatWiresModePackage(const HeatWiresModePackage *const package) {

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
                           SetNetworkGeometryPackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_EAST);
        } else if (routeToSouth) {
            __relayPackage((Package *) package, &ParticleAttributes.directionOrientedPorts.south,
                           SetNetworkGeometryPackageBufferPointerSize,
                           STATE_TYPE_SENDING_PACKAGE_TO_SOUTH);
        }
    }

    ParticleAttributes.protocol.isBroadcastEnabled = package->header.enableBroadcast;
    ParticleAttributes.actuationCommand.actuationPower.dutyCycleLevel = package->heatMode;
}
