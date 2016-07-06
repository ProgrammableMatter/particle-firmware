//
// Created by Raoul Rubien on 05.07.16
//

/**
 * implementation of in buffer constructing tx/rx protocol package
 */

#pragma once

#include "./CommunicationProtocol.h"
#include "./CommunicationProtocolTypes.h"
#include "uc-core/particle/Globals.h"
#include "uc-core/interrupts/TimerCounter.h"

extern CTOR_ATTRS void constructSendEnumeratePackage(volatile TxPort *txPort,
                                                     uint8_t localAddressRow,
                                                     uint8_t localAddressColumn);
/**
 * Constructs a SendEnumeratePackage at the beginning of parameter o.
 */
CTOR_ATTRS void constructSendEnumeratePackage(volatile TxPort *txPort, uint8_t localAddressRow,
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


extern CTOR_ATTRS void constructSendACKPackage(volatile TxPort *txPort);
/**
 * Constructs an empty ACK package at the given port tx buffer.
 */
CTOR_ATTRS void constructSendACKPackage(volatile TxPort *txPort) {
//    PackageHeader *ph = &((Package *)txPort->buffer.bytes)->asACKPackage;
    Package *package = (Package *) txPort->buffer.bytes;
    package->asACKPackage.__startBit = 1;
    package->asACKPackage.headerId = PACKAGE_HEADER_ID_TYPE_ACK;
    setBufferDataEndPointer(txPort->dataEndPos, AckPackagePointerSize);
}

extern CTOR_ATTRS void constructSendEnumeratedACKWithAddressToParentPackage(void);
/**
 * Constructs an ACK package with 2 byte data payload as the address field at the given port tx buffer.
 */
CTOR_ATTRS void constructSendEnumeratedACKWithAddressToParentPackage(void) {
    Package *package = (Package *) ParticleAttributes.communication.ports.tx.north.buffer.bytes;
    package->asACKWithLocalAddress.__startBit = 1;
    package->asACKWithLocalAddress.headerId = PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA;
    package->asACKWithLocalAddress.addressRow0 = ParticleAttributes.node.address.row;
    package->asACKWithLocalAddress.addressColumn0 = ParticleAttributes.node.address.column;
    setBufferDataEndPointer(ParticleAttributes.communication.ports.tx.north.dataEndPos,
                            AckWithAddressPackageBufferPointerSize);
}

extern CTOR_ATTRS void constructSendSyncTimePackage(volatile TxPort *txPort);
/**
 * constructs a package containing the current uint16_t local time as payload
 */
CTOR_ATTRS void constructSendSyncTimePackage(volatile TxPort *txPort) {
    Package *package = (Package *) txPort->buffer.bytes;
    package->asSyncTimePackage.__startBit = 1;
    package->asSyncTimePackage.headerId = PACKAGE_HEADER_ID_TYPE_SYNC_TIME;
    package->asSyncTimePackage.time = TIMER_TX_RX_COUNTER_VALUE;
    setBufferDataEndPointer(txPort->dataEndPos, TimePackageBufferPointerSize);
}

extern CTOR_ATTRS void constructSendAnnounceNetworkGeometryPackage(uint8_t row, uint8_t column);
/**
 * constructs a network geometry announcement package containing the local address
 */
CTOR_ATTRS void constructSendAnnounceNetworkGeometryPackage(uint8_t row, uint8_t column) {
    Package *package = (Package *) &ParticleAttributes.communication.ports.tx.north;
    package->asAnnounceNetworkGeometryPackage.__startBit = 1;
    package->asAnnounceNetworkGeometryPackage.headerId = PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE;
    package->asAnnounceNetworkGeometryPackage.rows = row;
    package->asAnnounceNetworkGeometryPackage.columns = column;
    setBufferDataEndPointer(ParticleAttributes.communication.ports.tx.north.dataEndPos,
                            AnnounceNetworkGeometryPackageBufferPointerSize);
}