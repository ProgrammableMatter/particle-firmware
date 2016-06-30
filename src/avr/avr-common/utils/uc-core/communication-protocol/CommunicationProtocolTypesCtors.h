//
// Created by Raoul Rubien on 11.05.16
//

/**
 * implementation of tx/rx protocol package functions
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
    Package *package = (Package *) txPort->buffer.bytes;
    PackageHeader *pha = &package->asACKPackage;
    pha->__startBit = 1;
    pha->headerId = PACKAGE_HEADER_ID_TYPE_ACK;
    setBufferDataEndPointer(txPort->dataEndPos, PackageHeaderBufferPointerSize);
}

extern CTOR_ATTRS void constructSendEnumeratedACKWithAddressToParent(void);
/**
 * Constructs an ACK package with 2 byte data payload as the address field at the given port tx buffer.
 */
CTOR_ATTRS void constructSendEnumeratedACKWithAddressToParent(void) {
    Package *package = (Package *) ParticleAttributes.ports.tx.north.buffer.bytes;
    PackageHeaderAddress *pha = &package->asACKWithLocalAddress;
    pha->__startBit = 1;
    pha->headerId = PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA;
    pha->addressRow0 = ParticleAttributes.node.address.row;
    pha->addressColumn0 = ParticleAttributes.node.address.column;
    setBufferDataEndPointer(ParticleAttributes.ports.tx.north.dataEndPos,
                            PackageHeaderData19BufferPointerSize);
}


extern CTOR_ATTRS void constructCommunicationProtocol(volatile CommunicationProtocolState *o);

CTOR_ATTRS void constructCommunicationProtocol(volatile CommunicationProtocolState *o) {
    o->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
    o->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE;
    o->stateTimeoutCounter = 0;
}