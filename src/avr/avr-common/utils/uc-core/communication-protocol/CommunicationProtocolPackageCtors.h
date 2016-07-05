//
// Created by Raoul Rubien on 05.07.16
//

/**
 * implementation of in buffer constructing tx/rx protocol package
 */

#pragma once

#include "uc-core/particle/Globals.h"
#include "./CommunicationProtocol.h"
#include "./CommunicationProtocolTypes.h"

extern CTOR_ATTRS void constructSendEnumeratePackage(volatile TxPort *txPort,
                                                     uint8_t localAddressRow,
                                                     uint8_t localAddressColumn);
/**
 * Constructs a SendEnumeratePackage at the beginning of parameter o.
 */
CTOR_ATTRS void constructSendEnumeratePackage(volatile TxPort *txPort, uint8_t localAddressRow,
                                              uint8_t localAddressColumn) {
    Package *package = (Package *) txPort->buffer.bytes;
    PackageHeaderAddress *pha = &package->asEnumerationPackage;

    pha->__startBit = 1;
    pha->headerIsStream = false;
    pha->headerIsCommand = true;
    pha->headerIsBroadcast = false;
    pha->headerId = PACKAGE_HEADER_ID_TYPE_ENUMERATE;
    pha->addressRow0 = localAddressRow;
    pha->addressColumn0 = localAddressColumn;

    setBufferDataEndPointer(txPort->dataEndPos, PackageHeaderAddressBufferPointerSize);
}


extern CTOR_ATTRS void constructSendACKPackage(volatile TxPort *txPort);
/**
 * Constructs an empty ACK package at the given port tx buffer.
 */
CTOR_ATTRS void constructSendACKPackage(volatile TxPort *txPort) {
//    PackageHeader *ph = &((Package *)txPort->buffer.bytes)->asACKPackage;
    Package *package = (Package *) txPort->buffer.bytes;
    PackageHeader *ph = &package->asACKPackage;
    ph->__startBit = 1;
    ph->headerId = PACKAGE_HEADER_ID_TYPE_ACK;
    setBufferDataEndPointer(txPort->dataEndPos, PackageHeaderBufferPointerSize);
}

extern CTOR_ATTRS void constructSendEnumeratedACKWithAddressToParentPackage(void);
/**
 * Constructs an ACK package with 2 byte data payload as the address field at the given port tx buffer.
 */
CTOR_ATTRS void constructSendEnumeratedACKWithAddressToParentPackage(void) {
    Package *package = (Package *) ParticleAttributes.communication.ports.tx.north.buffer.bytes;
    PackageHeaderAddress *pha = &package->asACKWithLocalAddress;
    pha->__startBit = 1;
    pha->headerId = PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA;
    pha->addressRow0 = ParticleAttributes.node.address.row;
    pha->addressColumn0 = ParticleAttributes.node.address.column;
    setBufferDataEndPointer(ParticleAttributes.communication.ports.tx.north.dataEndPos,
                            PackageHeaderAddressBufferPointerSize);
}

extern CTOR_ATTRS void constructSendSyncTimePackage(volatile TxPort *txPort);
/**
 * constructs a package containing the current uint16_t local time as payload
 */
CTOR_ATTRS void constructSendSyncTimePackage(volatile TxPort *txPort) {
    Package *package = (Package *) txPort->buffer.bytes;
    PackageHeaderTime *pht = &package->asSyncTimePackage;
    pht->__startBit = 1;
    pht->headerId = PACKAGE_HEADER_ID_TYPE_SYNC_TIME;
    setBufferDataEndPointer(txPort->dataEndPos, PackageHeaderTimeBufferPointerSize);
}