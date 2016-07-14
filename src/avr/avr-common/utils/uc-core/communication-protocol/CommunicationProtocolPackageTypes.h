/**
 * @author Raoul Rubien  13.07.16.
 */

/**
 * definition of all package types that can be transmitted/received
 */

#pragma once

#include <stdint.h>

/**
 * describes possible header IDs. Note the enum values must not exceed uint8_t max.
 */
typedef enum PackageHeaderId {
    PACKAGE_HEADER_ID_TYPE_ENUMERATE = 0,
    PACKAGE_HEADER_ID_TYPE_HEAT_WIRES = 1,
    __PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE = 2,
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
    uint8_t __headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit: 1;
    uint8_t __pad: 1;
} HeaderPackage;

/**
 * HeaderPackage data length expressed as BufferPointer
 */
#define HeaderPackagePointerSize \
    ((((uint16_t) 0x0100) << 6) \
    |((uint16_t) 0x0000))

/**
 * describes a package header
 */
typedef struct AckPackage {
    uint8_t startBit : 1;
    uint8_t __headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit: 1;
    uint8_t enableBroadcast : 1;
} AckPackage;

/**
 * AckPackage data length expressed as BufferPointer
 */
#define AckPackagePointerSize \
    ((((uint16_t) 0x0100) << 7) \
    |((uint16_t) 0x0000))

/**
 * describes a package header with subsequent address
 */
typedef struct AckWithAddressPackage {
    uint8_t startBit : 1;
    uint8_t __headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
} AckWithAddressPackage;

/**
 * PackageHeaderAddress length expressed as BufferPointer
 */
#define AckWithAddressPackageBufferPointerSize \
    ((((uint16_t) 0x0100) << 0)\
    |((uint16_t) 0x0003))


/**
 * describes a header with subsequent network geometry
 */
typedef struct AnnounceNetworkGeometryPackage {
    uint8_t startBit : 1;
    uint8_t __headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    uint8_t rows : 8;
    uint8_t columns : 8;
} AnnounceNetworkGeometryPackage;

/**
 * AnnounceNetworkGeometryPackage length expressed as BufferPointer
 */
#define AnnounceNetworkGeometryPackageBufferPointerSize \
    ((((uint16_t) 0x0100) << 0)\
    |((uint16_t) 0x0003))

/**
 * describes a header with subsequent network geometry
 */
typedef struct SetNetworkGeometryPackage {
    uint8_t startBit : 1;
    uint8_t __headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit: 1;
    uint8_t enableBroadcast : 1;
    uint8_t rows : 8;
    uint8_t columns : 8;
} SetNetworkGeometryPackage;

/**
 * SetNetworkGeometryPackage length expressed as BufferPointer
 */
#define SetNetworkGeometryPackageBufferPointerSize \
    ((((uint16_t) 0x0100) << 0)\
    |((uint16_t) 0x0003))

/**
 * describes a package header with subsequent enumeration address and bread crumb flag
 */
typedef struct EnumerationPackage {
    uint8_t startBit : 1;
    uint8_t __headerIsStream : 1;
    uint8_t headerId : 4;
    /**
     * Bread crumb is passed until the highest network address is reached.
     */
    uint8_t hasNetworkGeometryDiscoveryBreadCrumb : 1;
    uint8_t __pad : 1;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
} EnumerationPackage;

/**
 * PackageHeaderAddress length expressed as BufferPointer
 */
#define EnumerationPackageBufferPointerSize \
    ((((uint16_t) 0x0100) << 0) \
    |((uint16_t) 0x0003))


///**
// * describes a package header with subsequent address range
// */
//typedef struct PackageHeaderAddressRange {
//    uint8_t startBit : 1;
//    uint8_t __headerIsStream : 1;
//    uint8_t headerId : 4;
//    uint8_t __parityBit : 1;
//    uint8_t enableBroadcast : 1;
//    uint8_t addressRow0 : 8;
//    uint8_t addressColumn0 : 8;
//    uint8_t addressRow1 : 8;
//    uint8_t addressColumn1 : 8;
//} PackageHeaderAddressRange;

///**
// * PackageHeaderAddressRange length expressed as BufferPointer
// */
//#define PackageHeaderAddressRangeBufferPointerSize
//    ((((uint16_t) 0x0100) << 0)
//    |((uint16_t) 0x0005))


/**
 * describes a package header with uint16_t subsequent time field
 */
typedef struct TimePackage {
    uint8_t startBit : 1;
    uint8_t __headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
    uint16_t time;
    uint16_t packageTransmissionLatency;
    uint16_t stuffing;
} TimePackage;

/**
 * PackageHeaderTime length expressed as BufferPointer
 */
#define TimePackageBufferPointerSize  \
    ((((uint16_t) 0x0100) << 0) \
    |((uint16_t) 0x0007))

/**
 * describes a package header with subsequent start time stamp and duration
 */
typedef struct HeatWiresPackage {
    uint8_t startBit : 1;
    uint8_t __headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t __parityBit : 1;
    uint8_t enableBroadcast : 1;
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
#define HeatWiresPackageBufferPointerSize  \
    ((((uint16_t) 0x0100) << 0) \
    |((uint16_t) 0x0007))


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
} Package;
