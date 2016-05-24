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

//extern volatile ParticleAttributes;

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

/**
 * @param PackageType see types enclosed by Package union in CommunicationProtocolTypes.h
 * @param portCardinalDirection {north, east, south}
 * @param transmissionDirection {tx, rx}
 */
#define getXmissionPackage(__PackageType, __portCardinalDirection, __transmissionDirection) { \
    ((__PackageType *) ParticleAttributes.ports.__transmissionDirection.__portCardinalDirection.buffer.bytes) \
}

/**
 * Evaluates to the properly casted pointer pointing at the beginning of the transmission buffer.
 * @param PackageType see types enclosed by Package union in CommunicationProtocolTypes.h
 * @param portCardinalDirection {north, east, south}
 */
#define getTransmissionPackage(__PackageType, __portCardinalDirection) { \
    getXmissionPackage(__PackageType, __portCardinalDirection, tx) \
}

/**
 * Evaluates to the properly casted pointer pointing at the beginning of the reception buffer.
 * @param PackageType see types enclosed by Package union in CommunicationProtocolTypes.h
 * @param portCardinalDirection {north, east, south}
 */
#define getReceptionPackage(__PackageType, __portCardinalDirection) { \
    getXmissionPackage(__PackageType, __portCardinalDirection, rx) \
}

/**
 * Constructs a SendEnumeratePackage at the beginning of parameter o.
 */
FUNC_ATTRS void constructSendEnumeratePackage(volatile PackageHeaderAddress *o, uint8_t localAddressRow,
                                              uint8_t localAddressColumn) {
    o->headerIsStream = false;
    o->headerIsCommand = true;
    o->headerIsBroadcast = false;
    o->addressRow0 = localAddressRow;
    o->addressColumn0 = localAddressColumn;
}

/**
 * Constructs a SendEnumeratePackage at the beginning of the north tx-buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageNorth(uint8_t localAddressRow, uint8_t localAddressColumn) {
    volatile Package *package = (Package *) ParticleAttributes.ports.tx.north.buffer.bytes;
    volatile PackageHeaderAddress *pha = &package->asDedicatedHeader;
    constructSendEnumeratePackage(pha, localAddressRow, localAddressColumn);
    ParticleAttributes.ports.tx.north.dataEndPos = PackageHeaderAddressBufferPointerSize;
}

/**
 * Constructs a SendEnumeratePackage at the beginning of the east tx-buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageEast(uint8_t localAddressRow, uint8_t localAddressColumn) {
    Package *package = (Package *) ParticleAttributes.ports.tx.east.buffer.bytes;
    PackageHeaderAddress *pha = &package->asDedicatedHeader;
    constructSendEnumeratePackage(pha, localAddressRow, localAddressColumn);
    ParticleAttributes.ports.tx.east.dataEndPos = PackageHeaderAddressBufferPointerSize;
}

/**
 * Constructs a SendEnumeratePackage at the beginning of the south tx-buffer.
 */
FUNC_ATTRS void constructSendEnumeratePackageSouth(uint8_t localAddressRow, uint8_t localAddressColumn) {
    Package *package = (Package *) ParticleAttributes.ports.tx.south.buffer.bytes;
    PackageHeaderAddress *pha = &package->asDedicatedHeader;
    constructSendEnumeratePackage(pha, localAddressRow, localAddressColumn);
    ParticleAttributes.ports.tx.south.dataEndPos = PackageHeaderAddressBufferPointerSize;
}

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif