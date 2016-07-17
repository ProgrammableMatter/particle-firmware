/**
 * @author Raoul Rubien  13.07.16.
 */

/**
 * definition of all package types that can be transmitted/received
 */

#pragma once

#include <stdint.h>

#define __pointerBytes(numBytes) ((uint16_t) numBytes)
#define __pointerBits(numBits) (((uint16_t) 0x0100) << numBits)

/**
 * describes possible header IDs. Note the enum values must not exceed uint8_t max.
 */
typedef enum PackageHeaderId {
    PACKAGE_HEADER_ID_TYPE_ENUMERATE = 0,
    PACKAGE_HEADER_ID_TYPE_HEAT_WIRES = 1,
    PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE = 2,
    __UNUSED1 = 3,
    __UNUSED2 = 4,
    PACKAGE_HEADER_ID_TYPE_SET_NETWORK_GEOMETRY = 5,
    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE = 6,
    __UNUSED3 = 7,
    __PACKAGE_HEADER_ID_TYPE_RESET = 8,
    PACKAGE_HEADER_ID_TYPE_SYNC_TIME = 9,
    __PACKAGE_HEADER_ID_TYPE_PING_REQUEST = 10,
    __PACKAGE_HEADER_ID_TYPE_PING_RESPONSE = 11,
    __PACKAGE_HEADER_ID_TYPE_HEATING_MODE = 12,
    PACKAGE_HEADER_ID_TYPE_ACK = 13,
    PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA = 14,
    PACKAGE_HEADER_ID_TYPE_EXTENDED_HEADER = 15
} PackageHeaderId;

/**
 * describes a package header
 */
typedef struct HeaderPackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit: 1;
    uint8_t __pad: 2;
} HeaderPackage;

/**
 * HeaderPackage data length expressed as BufferPointer
 */
#define HeaderPackagePointerSize (__pointerBytes(0) | __pointerBits(6))

/**
 * describes a package header
 */
typedef struct AckPackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit: 1;
    uint8_t enableBroadcast : 1;
    uint8_t __pad : 1;
} AckPackage;

/**
 * AckPackage data length expressed as BufferPointer
 */
#define AckPackagePointerSize (__pointerBytes(0) | __pointerBits(7))

/**
 * describes a package header with subsequent address
 */
typedef struct AckWithAddressPackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    uint8_t __unused : 1;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
} AckWithAddressPackage;

/**
 * PackageHeaderAddress length expressed as BufferPointer
 */
#define AckWithAddressPackageBufferPointerSize (__pointerBytes(3) | __pointerBits(0))

/**
 * describes a header with subsequent network geometry
 */
typedef struct AnnounceNetworkGeometryPackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    uint8_t __unused : 1;
    uint8_t rows : 8;
    uint8_t columns : 8;
} AnnounceNetworkGeometryPackage;

/**
 * AnnounceNetworkGeometryPackage length expressed as BufferPointer
 */
#define AnnounceNetworkGeometryPackageBufferPointerSize (__pointerBytes(3) | __pointerBits(0))

/**
 * describes a header with subsequent network geometry
 */
typedef struct SetNetworkGeometryPackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit: 1;
    uint8_t enableBroadcast : 1;
    uint8_t __unused : 1;
    uint8_t rows : 8;
    uint8_t columns : 8;
} SetNetworkGeometryPackage;

/**
 * SetNetworkGeometryPackage length expressed as BufferPointer
 */
#define SetNetworkGeometryPackageBufferPointerSize (__pointerBytes(3) | __pointerBits(0))

/**
 * describes a package header with subsequent enumeration address and bread crumb flag
 */
typedef struct EnumerationPackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    /**
     * Bread crumb is passed until the highest network address is reached.
     */
    uint8_t hasNetworkGeometryDiscoveryBreadCrumb : 1;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
} EnumerationPackage;

/**
 * PackageHeaderAddress length expressed as BufferPointer
 */
#define EnumerationPackageBufferPointerSize (__pointerBytes(3) | __pointerBits(0))

/**
 * describes a package header with uint16_t subsequent time field
 */
typedef struct TimePackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    uint8_t __unused : 1;
    uint16_t time;
    uint16_t packageTransmissionLatency;
    uint16_t stuffing;
} TimePackage;

/**
 * PackageHeaderTime length expressed as BufferPointer
 */
#define TimePackageBufferPointerSize (__pointerBytes(7) | __pointerBits(0))

/**
 * describes a package header with subsequent address, start time stamp, duration and wires flags
 */
typedef struct HeatWiresPackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    uint8_t __unused : 1;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint16_t startTimeStamp : 16;
    uint8_t duration : 8;
    uint8_t durationMsb : 2;
    uint8_t northLeft : 1;
    uint8_t northRight: 1;
    uint8_t __pad: 4;
} HeatWiresPackage;

/**
 * HeatWiresPackage length expressed as BufferPointer
 */
#define HeatWiresPackageBufferPointerSize (__pointerBytes(6) | __pointerBits(4))

/**
 * describes a package header with subsequent addres range, start time stamp, duration and wires flags
 */
typedef struct HeatWiresRangePackage {
    uint8_t startBit : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    uint8_t __unused : 1;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
    uint16_t startTimeStamp : 16;
    uint8_t duration : 8;
    uint8_t durationMsb : 2;
    uint8_t northLeft : 1;
    uint8_t northRight: 1;
    uint8_t __pad: 4;
} HeatWiresRangePackage;

/**
 * HeatWiresRangePackage length expressed as BufferPointer
 */
#define HeatWiresRangePackageBufferPointerSize (__pointerBytes(8) | __pointerBits(4))

/**
 * union for a convenient way to access buffered packages
 */
typedef union Package {
    HeaderPackage asHeader;
    AckPackage asACKPackage;
    AckWithAddressPackage asACKWithLocalAddress;
    AckWithAddressPackage asACKWithRemoteAddress;
    EnumerationPackage asEnumerationPackage;
    TimePackage asSyncTimePackage;
    AnnounceNetworkGeometryPackage asAnnounceNetworkGeometryPackage;
    SetNetworkGeometryPackage asSetNetworkGeometryPackage;
    HeatWiresPackage asHeatWiresPackage;
    HeatWiresRangePackage asHeatWiresRangePackage;
} Package;
