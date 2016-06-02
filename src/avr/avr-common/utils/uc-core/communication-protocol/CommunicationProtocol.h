//
// Created by Raoul Rubien on 11.05.16
//

/**
 * implementation of tx/rx protocol package functions
 */
#ifndef __COMMUNICATION_PROTOCOL_PROTOCOL_H__
#define __COMMUNICATION_PROTOCOL_PROTOCOL_H__

#include <uc-core/Globals.h>
#include "CommunicationProtocolTypes.h"

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

/**
 * Constructs a SendEnumeratePackage at the beginning of parameter o.
 */
FUNC_ATTRS void __constructSendEnumeratePackage(volatile PackageHeaderAddress *o, uint8_t localAddressRow,
                                                uint8_t localAddressColumn) {
    o->headerIsStream = false;
    o->headerIsCommand = true;
    o->headerIsBroadcast = false;
    o->headerId = PACKAGE_HEADER_ID_TYPE_ENUMERATE;
    o->addressRow0 = localAddressRow;
    o->addressColumn0 = localAddressColumn;
}

/**
 * Constructs a SendEnumeratePackage at the north tx buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageNorth(uint8_t localAddressRow,
                                                   uint8_t localAddressColumn) {
    volatile Package *package = (Package *) ParticleAttributes.ports.tx.north.buffer.bytes;
    volatile PackageHeaderAddress *pha = &package->asDedicatedHeader;
    __constructSendEnumeratePackage(pha, localAddressRow,
                                    localAddressColumn);
    ParticleAttributes.ports.tx.north.dataEndPos = PackageHeaderAddressBufferPointerSize;
}

/**
 * Constructs a SendEnumeratePackage at the east tx buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageEast(uint8_t localAddressRow,
                                                  uint8_t localAddressColumn) {
    Package *package = (Package *) ParticleAttributes.ports.tx.east.buffer.bytes;
    PackageHeaderAddress *pha = &package->asDedicatedHeader;
    __constructSendEnumeratePackage(pha, localAddressRow, localAddressColumn);
    ParticleAttributes.ports.tx.east.dataEndPos = PackageHeaderAddressBufferPointerSize;
}

/**
 * Constructs a SendEnumeratePackage at the south tx buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageSouth(uint8_t localAddressRow, uint8_t localAddressColumn) {
    Package *package = (Package *) ParticleAttributes.ports.tx.south.buffer.bytes;
    PackageHeaderAddress *pha = &package->asDedicatedHeader;
    __constructSendEnumeratePackage(pha, localAddressRow,
                                    localAddressColumn);
    ParticleAttributes.ports.tx.south.dataEndPos = PackageHeaderAddressBufferPointerSize;
}

/**
 * Constructs an empty ACK package at the given port tx buffer.
 */
FUNC_ATTRS void constructSendACKPackage(volatile TxPort *txPort) {
    Package *package = (Package *) txPort->buffer.bytes;
    PackageHeaderAddress *pha = &package->asDedicatedHeader;
    pha->headerId = PACKAGE_HEADER_ID_TYPE_ACK;
    txPort->dataEndPos = PackageHeaderBufferPointerSize;
}

/**
 * Constructs an ACK package with 2 byte data payload (the local address) at the north tx buffer.
 */
FUNC_ATTRS void constructSendEnumeratedACKWithAddressToNorth(void) {
    Package *package = (Package *) ParticleAttributes.ports.tx.north.buffer.bytes;
    PackageHeaderData19 *pha = &package->asACKData19;
    pha->headerId = PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA;
    pha->dataLsb = ParticleAttributes.node.address.row;
    pha->dataCeb = ParticleAttributes.node.address.column;
    ParticleAttributes.ports.tx.north.dataEndPos = PackageHeaderData19BufferPointerSize;
}


/**
 * invalidates all reception buffers
 */
FUNC_ATTRS void clearReceptionBuffers(void) {
    ParticleAttributes.ports.rx.north.isDataBuffered = false;
    ParticleAttributes.ports.rx.north.isReceiving = false;
    ParticleAttributes.ports.rx.north.isOverflowed = false;

    ParticleAttributes.ports.rx.east.isDataBuffered = false;
    ParticleAttributes.ports.rx.east.isReceiving = false;
    ParticleAttributes.ports.rx.east.isOverflowed = false;

    ParticleAttributes.ports.rx.south.isDataBuffered = false;
    ParticleAttributes.ports.rx.south.isReceiving = false;
    ParticleAttributes.ports.rx.south.isOverflowed = false;
}

/**
 * invalidates the given port's reception buffer
 */
FUNC_ATTRS void clearReceptionBuffer(volatile RxPort *o) {
    o->isDataBuffered = false;
    o->isReceiving = false;
    o->isOverflowed = false;
    IF_SIMULATION_CHAR_OUT('c');
}


/**
 * prepares the given transmission port for buffering and later transmission
 */
FUNC_ATTRS void prepareTransmissionPortBuffer(volatile TxPort *o) {
    o->enableTransmission = false;
    o->retainTransmission = true;
    o->isTransmitting = false;
    bufferBitPointerStart(&o->buffer.pointer);
}

/**
 * sets flags to enable transmission of the given port
 */
FUNC_ATTRS void releaseTransmissionPortBufferForTransmission(volatile TxPort *port) {
//    DELAY_US_150;
    port->retainTransmission = true;
    port->enableTransmission = true;
}


/**
 * returns true if the given port is in transmission mode
 */
FUNC_ATTRS uint8_t isPortTransmitting(volatile TxPort *o) {
    return o->isTransmitting || o->enableTransmission;
}

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif