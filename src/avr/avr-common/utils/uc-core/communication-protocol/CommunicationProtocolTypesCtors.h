//
// Created by Raoul Rubien on 11.05.16
//

/**
 * implementation of tx/rx protocol package functions
 */
#pragma once

#include "uc-core/Globals.h"
#include "CommunicationProtocolTypes.h"

extern FUNC_ATTRS void constructSendEnumeratePackage(volatile PackageHeaderAddress *o,
                                                     uint8_t localAddressRow,
                                                     uint8_t localAddressColumn);
/**
 * Constructs a SendEnumeratePackage at the beginning of parameter o.
 */
FUNC_ATTRS void constructSendEnumeratePackage(volatile PackageHeaderAddress *o, uint8_t localAddressRow,
                                              uint8_t localAddressColumn) {
    o->headerIsStream = false;
    o->headerIsCommand = true;
    o->headerIsBroadcast = false;
    o->headerId = PACKAGE_HEADER_ID_TYPE_ENUMERATE;
    o->addressRow0 = localAddressRow;
    o->addressColumn0 = localAddressColumn;
}

extern FUNC_ATTRS void constructSendEnumeratePackageNorth(uint8_t localAddressRow,
                                                          uint8_t localAddressColumn);
/**
 * Constructs a SendEnumeratePackage at the north tx buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageNorth(uint8_t localAddressRow,
                                                   uint8_t localAddressColumn) {
    volatile Package *package = (Package *) ParticleAttributes.ports.tx.north.buffer.bytes;
    volatile PackageHeaderAddress *pha = &package->asEnumerationPackage;
    constructSendEnumeratePackage(pha, localAddressRow,
                                  localAddressColumn);
    setBufferDataEndPointer(ParticleAttributes.ports.tx.north.dataEndPos,
                            PackageHeaderAddressBufferPointerSize);
}

extern FUNC_ATTRS void constructSendEnumeratePackageEast(uint8_t localAddressRow,
                                                         uint8_t localAddressColumn);
/**
 * Constructs a SendEnumeratePackage at the east tx buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageEast(uint8_t localAddressRow,
                                                  uint8_t localAddressColumn) {
    Package *package = (Package *) ParticleAttributes.ports.tx.east.buffer.bytes;
    PackageHeaderAddress *pha = &package->asEnumerationPackage;
    constructSendEnumeratePackage(pha, localAddressRow, localAddressColumn);
    setBufferDataEndPointer(ParticleAttributes.ports.tx.east.dataEndPos,
                            PackageHeaderAddressBufferPointerSize);
}

extern FUNC_ATTRS void constructSendEnumeratePackageSouth(uint8_t localAddressRow,
                                                          uint8_t localAddressColumn);
/**
 * Constructs a SendEnumeratePackage at the south tx buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageSouth(uint8_t localAddressRow, uint8_t localAddressColumn) {
    Package *package = (Package *) ParticleAttributes.ports.tx.south.buffer.bytes;
    PackageHeaderAddress *pha = &package->asEnumerationPackage;
    constructSendEnumeratePackage(pha, localAddressRow,
                                  localAddressColumn);
    setBufferDataEndPointer(ParticleAttributes.ports.tx.south.dataEndPos,
                            PackageHeaderAddressBufferPointerSize);
}

extern FUNC_ATTRS void constructSendACKPackage(volatile TxPort *txPort);
/**
 * Constructs an empty ACK package at the given port tx buffer.
 */
FUNC_ATTRS void constructSendACKPackage(volatile TxPort *txPort) {
    Package *package = (Package *) txPort->buffer.bytes;
    PackageHeader *pha = &package->asACKPackage;
    pha->headerId = PACKAGE_HEADER_ID_TYPE_ACK;
    setBufferDataEndPointer(txPort->dataEndPos, PackageHeaderBufferPointerSize);
}

extern FUNC_ATTRS void constructSendEnumeratedACKWithAddressToParent(void);
/**
 * Constructs an ACK package with 2 byte data payload as the address field at the given port tx buffer.
 */
FUNC_ATTRS void constructSendEnumeratedACKWithAddressToParent(void) {
    Package *package = (Package *) ParticleAttributes.ports.tx.north.buffer.bytes;
    PackageHeaderAddress *pha = &package->asACKWithLocalAddress;
    pha->headerId = PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA;
    pha->addressRow0 = ParticleAttributes.node.address.row;
    pha->addressColumn0 = ParticleAttributes.node.address.column;
    setBufferDataEndPointer(ParticleAttributes.ports.tx.north.dataEndPos,
                            PackageHeaderData19BufferPointerSize);
}
