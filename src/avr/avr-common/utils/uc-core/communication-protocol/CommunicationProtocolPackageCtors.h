//
// Created by Raoul Rubien on 05.07.16
//

/**
 * implementation of in buffer constructing tx/rx protocol package
 */

#pragma once

#include "./CommunicationProtocol.h"
#include "./CommunicationProtocolPackageTypes.h"
#include "uc-core/particle/Globals.h"
#include "uc-core/configuration/interrupts/TimerCounter.h"

extern CTOR_ATTRS void constructEnumeratePackage(volatile TxPort *txPort,
                                                 uint8_t localAddressRow,
                                                 uint8_t localAddressColumn);
/**
 * constructor function: builds the protocol package at the given port's buffer
 */
CTOR_ATTRS void constructEnumeratePackage(volatile TxPort *txPort, uint8_t localAddressRow,
                                          uint8_t localAddressColumn) {
    Package *package = (Package *) txPort->buffer.bytes;

    package->asEnumerationPackage.__startBit = 1;

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
    package->asEnumerationPackage.headerId = PACKAGE_HEADER_ID_TYPE_ENUMERATE;
    package->asEnumerationPackage.addressRow0 = localAddressRow;
    package->asEnumerationPackage.addressColumn0 = localAddressColumn;
    setBufferDataEndPointer(txPort->dataEndPos, EnumerationPackageBufferPointerSize);
}


extern CTOR_ATTRS void constructEnumerationACKPackage(volatile TxPort *txPort);
/**
 * constructor function: builds the protocol package at the given port's buffer
 */
CTOR_ATTRS void constructEnumerationACKPackage(volatile TxPort *txPort) {
    Package *package = (Package *) txPort->buffer.bytes;
    package->asACKPackage.__startBit = 1;
    package->asACKPackage.enableBroadcast = true;
    package->asACKPackage.headerId = PACKAGE_HEADER_ID_TYPE_ACK;
    setBufferDataEndPointer(txPort->dataEndPos, AckPackagePointerSize);

}

extern CTOR_ATTRS void constructEnumerationACKWithAddressToParentPackage(void);
/**
 * constructor function: builds the protocol package at the given port's buffer
 */
CTOR_ATTRS void constructEnumerationACKWithAddressToParentPackage(void) {
    Package *package = (Package *) ParticleAttributes.communication.ports.tx.north.buffer.bytes;
    package->asACKWithLocalAddress.__startBit = 1;
    package->asACKWithLocalAddress.headerId = PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA;
    package->asACKWithLocalAddress.addressRow0 = ParticleAttributes.node.address.row;
    package->asACKWithLocalAddress.addressColumn0 = ParticleAttributes.node.address.column;
    setBufferDataEndPointer(ParticleAttributes.communication.ports.tx.north.dataEndPos,
                            AckWithAddressPackageBufferPointerSize);
}

extern CTOR_ATTRS void constructSyncTimePackage(volatile TxPort *txPort);
/**
 * constructor function: builds the protocol package at the given port's buffer
 */
CTOR_ATTRS void constructSyncTimePackage(volatile TxPort *txPort) {
    Package *package = (Package *) txPort->buffer.bytes;
    package->asSyncTimePackage.__startBit = 1;
    package->asSyncTimePackage.headerId = PACKAGE_HEADER_ID_TYPE_SYNC_TIME;
    package->asSyncTimePackage.enableBroadcast = true;
    package->asSyncTimePackage.time = TIMER_TX_RX_COUNTER_VALUE;
    package->asSyncTimePackage.packageTransmissionLatency = COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PACKAGE_RECEPTION_DURATION;
    package->asSyncTimePackage.stuffing = 0x5555;
    setBufferDataEndPointer(txPort->dataEndPos, TimePackageBufferPointerSize);
//    DEBUG_INT16_OUT(TIMER_TX_RX_COUNTER_VALUE);
}

extern CTOR_ATTRS void constructAnnounceNetworkGeometryPackage(uint8_t row, uint8_t column);
/**
 * constructor function: builds the protocol package at the given port's buffer
 */
CTOR_ATTRS void constructAnnounceNetworkGeometryPackage(uint8_t row, uint8_t column) {
    Package *package = (Package *) &ParticleAttributes.communication.ports.tx.north.buffer.bytes;
    package->asAnnounceNetworkGeometryPackage.__startBit = 1;
    package->asAnnounceNetworkGeometryPackage.headerId = PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE;
    package->asAnnounceNetworkGeometryPackage.enableBroadcast = true;
    package->asAnnounceNetworkGeometryPackage.rows = row;
    package->asAnnounceNetworkGeometryPackage.columns = column;
    setBufferDataEndPointer(ParticleAttributes.communication.ports.tx.north.dataEndPos,
                            AnnounceNetworkGeometryPackageBufferPointerSize);
}


extern CTOR_ATTRS void constructSetNetworkGeometryPackage(volatile TxPort *txPort, uint8_t row,
                                                          uint8_t column);
/**
 * constructor function: builds the protocol package at the given port's buffer
 */
CTOR_ATTRS void constructSetNetworkGeometryPackage(volatile TxPort *txPort, uint8_t row, uint8_t column) {
    Package *package = (Package *) &txPort->buffer.bytes;
    package->asSetNetworkGeometryPackage.__startBit = 1;
    package->asSetNetworkGeometryPackage.headerId = PACKAGE_HEADER_ID_TYPE_SET_NETWORK_GEOMETRY;
    package->asSetNetworkGeometryPackage.enableBroadcast = true;
    package->asSetNetworkGeometryPackage.rows = row;
    package->asSetNetworkGeometryPackage.columns = column;
    setBufferDataEndPointer(txPort->dataEndPos, SetNetworkGeometryPackageBufferPointerSize);
}

extern FUNC_ATTRS void constructHeatWiresPackage(volatile TxPort *txPort, uint16_t startTimeStamp,
                                                 uint16_t duration);
/**
 * constructor function: builds the protocol package at the given port's buffer
 */
FUNC_ATTRS void constructHeatWiresPackage(volatile TxPort *txPort, uint16_t startTimeStamp,
                                          uint16_t duration) {
    Package *package = (Package *) txPort->buffer.bytes;
    package->asHeatWiresPackage.__startBit = 1;
    package->asHeatWiresPackage.headerId = PACKAGE_HEADER_ID_TYPE_HEAT_WIRES;
    package->asHeatWiresPackage.enableBroadcast = false;
    package->asHeatWiresPackage.startTimeStamp = startTimeStamp;
    package->asHeatWiresPackage.duration = duration;
}
