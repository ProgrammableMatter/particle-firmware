/**
 * Created by Raoul Rubien on 05.07.16
 *
 * In buffer construction of protocol package related implementation.
 */

#pragma once

#include "./CommunicationProtocol.h"
#include "./CommunicationProtocolPackageTypes.h"
#include "uc-core/particle/Globals.h"

///**
// * Constructor function: builds the protocol package at the given port's buffer.
// */
//extern CTOR_ATTRS void constructHeaderPackage(volatile TxPort *txPort, uint8_t headerId,
//                                              bool enableBroadcast);
//
//CTOR_ATTRS void constructHeaderPackage(volatile TxPort *txPort, uint8_t headerId, bool enableBroadcast) {
//    // TODO enhancement: calculate and set parity bit
//    clearTransmissionPortBuffer(txPort);
//    Package *package = (Package *) txPort->buffer.bytes;
//    package->asHeader.startBit = 1;
//    package->asHeader.headerId = headerId;
//    package->asHeader.enableBroadcast = enableBroadcast;
//    setBufferDataEndPointer(txPort->dataEndPos, HeaderPackagePointerSize);
//}

/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern CTOR_ATTRS void constructEnumeratePackage(volatile TxPort *txPort,
                                                 uint8_t localAddressRow,
                                                 uint8_t localAddressColumn);

CTOR_ATTRS void constructEnumeratePackage(volatile TxPort *txPort, uint8_t localAddressRow,
                                          uint8_t localAddressColumn) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(txPort);
    Package *package = (Package *) txPort->buffer.bytes;

    package->asEnumerationPackage.startBit = 1;
    package->asEnumerationPackage.headerId = PACKAGE_HEADER_ID_TYPE_ENUMERATE;
    package->asEnumerationPackage.enableBroadcast = false;

    // on local bread crumb available or origin node
    if (ParticleAttributes.protocol.hasNetworkGeometryDiscoveryBreadCrumb
        || ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        // on tx east or tx south and no east connectivity
        if (txPort == &ParticleAttributes.communication.ports.tx.east
            || (txPort == &ParticleAttributes.communication.ports.tx.south
                && !ParticleAttributes.discoveryPulseCounters.east.isConnected)) {
            package->asEnumerationPackage.hasNetworkGeometryDiscoveryBreadCrumb = true;
        } else {
            package->asEnumerationPackage.hasNetworkGeometryDiscoveryBreadCrumb = false;
        }
    }
    package->asEnumerationPackage.addressRow0 = localAddressRow;
    package->asEnumerationPackage.addressColumn0 = localAddressColumn;
    setBufferDataEndPointer(txPort->dataEndPos, EnumerationPackageBufferPointerSize);
}


/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern CTOR_ATTRS void constructEnumerationACKPackage(volatile TxPort *txPort);

CTOR_ATTRS void constructEnumerationACKPackage(volatile TxPort *txPort) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(txPort);
    Package *package = (Package *) txPort->buffer.bytes;
    package->asACKPackage.startBit = 1;
    package->asACKPackage.enableBroadcast = true;
    package->asACKPackage.headerId = PACKAGE_HEADER_ID_TYPE_ACK;
    setBufferDataEndPointer(txPort->dataEndPos, AckPackagePointerSize);

}

/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern CTOR_ATTRS void constructEnumerationACKWithAddressToParentPackage(void);

CTOR_ATTRS void constructEnumerationACKWithAddressToParentPackage(void) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(ParticleAttributes.directionOrientedPorts.north.txPort);
    Package *package = (Package *) ParticleAttributes.directionOrientedPorts.north.txPort->buffer.bytes;
    package->asACKWithLocalAddress.startBit = 1;
    package->asACKWithLocalAddress.headerId = PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA;
    package->asACKWithLocalAddress.addressRow0 = ParticleAttributes.node.address.row;
    package->asACKWithLocalAddress.addressColumn0 = ParticleAttributes.node.address.column;
    setBufferDataEndPointer(ParticleAttributes.communication.ports.tx.north.dataEndPos,
                            AckWithAddressPackageBufferPointerSize);
}

/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern CTOR_ATTRS void constructSyncTimePackage(volatile TxPort *txPort);

CTOR_ATTRS void constructSyncTimePackage(volatile TxPort *txPort) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(txPort);
    Package *package = (Package *) txPort->buffer.bytes;
    package->asSyncTimePackage.startBit = 1;
    package->asSyncTimePackage.headerId = PACKAGE_HEADER_ID_TYPE_SYNC_TIME;
    package->asSyncTimePackage.enableBroadcast = false;
    package->asSyncTimePackage.time = TIMER_TX_RX_COUNTER_VALUE;
    package->asSyncTimePackage.packageTransmissionLatency = COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PACKAGE_RECEPTION_DURATION;
    package->asSyncTimePackage.stuffing = 0x5555;
    setBufferDataEndPointer(txPort->dataEndPos, TimePackageBufferPointerSize);
//    DEBUG_INT16_OUT(TIMER_TX_RX_COUNTER_VALUE);
}

/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern CTOR_ATTRS void constructAnnounceNetworkGeometryPackage(uint8_t row, uint8_t column);

CTOR_ATTRS void constructAnnounceNetworkGeometryPackage(uint8_t row, uint8_t column) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(ParticleAttributes.directionOrientedPorts.north.txPort);
    Package *package = (Package *) ParticleAttributes.directionOrientedPorts.north.txPort->buffer.bytes;
    package->asAnnounceNetworkGeometryPackage.startBit = 1;
    package->asAnnounceNetworkGeometryPackage.headerId = PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE;
    package->asAnnounceNetworkGeometryPackage.enableBroadcast = true;
    package->asAnnounceNetworkGeometryPackage.rows = row;
    package->asAnnounceNetworkGeometryPackage.columns = column;
    setBufferDataEndPointer(ParticleAttributes.communication.ports.tx.north.dataEndPos,
                            AnnounceNetworkGeometryPackageBufferPointerSize);
}


/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern CTOR_ATTRS void constructSetNetworkGeometryPackage(volatile TxPort *txPort, uint8_t rows,
                                                          uint8_t columns);

CTOR_ATTRS void constructSetNetworkGeometryPackage(volatile TxPort *txPort, uint8_t rows, uint8_t columns) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(txPort);
    Package *package = (Package *) txPort->buffer.bytes;
    package->asSetNetworkGeometryPackage.startBit = 1;
    package->asSetNetworkGeometryPackage.headerId = PACKAGE_HEADER_ID_TYPE_SET_NETWORK_GEOMETRY;
    package->asSetNetworkGeometryPackage.enableBroadcast = true;
    package->asSetNetworkGeometryPackage.rows = rows;
    package->asSetNetworkGeometryPackage.columns = columns;
    setBufferDataEndPointer(txPort->dataEndPos, SetNetworkGeometryPackageBufferPointerSize);
}

/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern FUNC_ATTRS void constructHeatWiresPackage(volatile TxPort *txPort,
                                                 NodeAddress *address,
                                                 Actuators *wires,
                                                 uint16_t startTimeStamp,
                                                 uint16_t duration);

FUNC_ATTRS void constructHeatWiresPackage(volatile TxPort *txPort, NodeAddress *address,
                                          Actuators *wires,
                                          uint16_t startTimeStamp,
                                          uint16_t duration) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(txPort);
    Package *package = (Package *) txPort->buffer.bytes;
    package->asHeatWiresPackage.startBit = 1;
    package->asHeatWiresPackage.headerId = PACKAGE_HEADER_ID_TYPE_HEAT_WIRES;
    package->asHeatWiresPackage.enableBroadcast = false;
    package->asHeatWiresPackage.addressRow0 = address->row;
    package->asHeatWiresPackage.addressColumn0 = address->column;
    package->asHeatWiresPackage.startTimeStamp = startTimeStamp;
    package->asHeatWiresPackage.duration = duration & 0x00ff;
    package->asHeatWiresPackage.durationMsb = duration & 0x0300;
    package->asHeatWiresPackage.northLeft = wires->northLeft;
    package->asHeatWiresPackage.northRight = wires->northRight;
    setBufferDataEndPointer(txPort->dataEndPos, HeatWiresPackageBufferPointerSize);
}

/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern FUNC_ATTRS void constructHeatWiresRangePackage(volatile TxPort *txPort,
                                                      NodeAddress *nodeAddressTopLeft,
                                                      NodeAddress *nodeAddressBottomRight,
                                                      Actuators *wires,
                                                      uint16_t startTimeStamp,
                                                      uint16_t duration);

FUNC_ATTRS void constructHeatWiresRangePackage(volatile TxPort *txPort,
                                               NodeAddress *nodeAddressTopLeft,
                                               NodeAddress *nodeAddressBottomRight,
                                               Actuators *wires,
                                               uint16_t startTimeStamp,
                                               uint16_t duration) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(txPort);
    Package *package = (Package *) txPort->buffer.bytes;
    package->asHeatWiresRangePackage.startBit = 1;
    package->asHeatWiresRangePackage.headerId = PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE;
    package->asHeatWiresRangePackage.enableBroadcast = false;
    package->asHeatWiresRangePackage.addressRow0 = nodeAddressTopLeft->row;
    package->asHeatWiresRangePackage.addressColumn0 = nodeAddressTopLeft->column;
    package->asHeatWiresRangePackage.addressRow1 = nodeAddressBottomRight->row;
    package->asHeatWiresRangePackage.addressColumn1 = nodeAddressBottomRight->column;
    package->asHeatWiresRangePackage.startTimeStamp = startTimeStamp;
    package->asHeatWiresRangePackage.duration = duration & 0x00ff;
    package->asHeatWiresRangePackage.durationMsb = (duration & 0x0300) >> 8;
    package->asHeatWiresRangePackage.northLeft = wires->northLeft;
    package->asHeatWiresRangePackage.northRight = wires->northRight;
    setBufferDataEndPointer(txPort->dataEndPos, HeatWiresRangePackageBufferPointerSize);
}

/**
 * Constructor function: builds the protocol package at the given port's buffer.
 */
extern CTOR_ATTRS void constructHeaderPackage(volatile TxPort *txPort);

CTOR_ATTRS void constructHeaderPackage(volatile TxPort *txPort) {
    // TODO enhancement: calculate and set parity bit
    clearTransmissionPortBuffer(txPort);
    Package *package = (Package *) txPort->buffer.bytes;
    package->asHeader.startBit = 1;
    package->asSetNetworkGeometryPackage.headerId = PACKAGE_HEADER_ID_HEADER;
    package->asHeader.enableBroadcast = false;
    setBufferDataEndPointer(txPort->dataEndPos, HeaderPackagePointerSize);
}