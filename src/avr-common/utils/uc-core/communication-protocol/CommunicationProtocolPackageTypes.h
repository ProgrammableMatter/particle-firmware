/**
 * @author Raoul Rubien  13.07.2016
 *
 * Definition of all package types that can be transmitted/received.
 */

#pragma once

#include <stdint.h>

/**
 * Convenience macro: evaluate to the number of bytes bit mask
 */
#define __pointerBytes(numBytes) ((uint16_t) numBytes)

/**
 * Convenience macro: evaluates to the number of bits bit mask
 */
#define __pointerBits(numBits) (((uint16_t) 0x0100) << numBits)

/**
 * Describes possible header IDs. Note the enum values must not exceed uint8_t max.
 */

typedef enum PackageHeaderId {
    PACKAGE_HEADER_ID_HEADER = 1,
    __PACKAGE_HEADER_ID_TYPE_SYNC_NETWORK_TIME = 2,
    __PACKAGE_HEADER_ID_TYPE_RELAY_HEADER = 2,
    __PACKAGE_HEADER_ID_TYPE_RESET = 3,
    PACKAGE_HEADER_ID_TYPE_ACK = 4,
    PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA = 5,
    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE = 6,
    PACKAGE_HEADER_ID_TYPE_SET_NETWORK_GEOMETRY = 7,
    PACKAGE_HEADER_ID_TYPE_ENUMERATE = 8,
    PACKAGE_HEADER_ID_TYPE_SYNC_TIME = 9,
    PACKAGE_HEADER_ID_TYPE_HEAT_WIRES = 10,
    PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_MODE = 11,
    __UNUSED11 = 11,
    __UNUSED12 = 12,
    __UNUSED13 = 13,
    __UNUSED14 = 14,
    PACKAGE_HEADER_ID_TYPE_EXTENDED_HEADER = 15,
    __UNUSED00 = 0,
} PackageHeaderId;

/**
 * describes a package header
 */
typedef struct HeaderPackage {
    uint8_t startBit : 1;
    uint8_t id : 4;
    uint8_t isRangeCommand: 1;
    uint8_t parityBit: 1;
    uint8_t enableBroadcast : 1;
} HeaderPackage;

/**
 * HeaderPackage data length expressed as (uint16_t) BufferPointer
 */
#define HeaderPackagePointerSize (__pointerBytes(1) | __pointerBits(0))


/**
 * describes an acknowledge package
 */
typedef HeaderPackage AckPackage;

/**
 * describes a package that is automatically relayed on the east and south port
 * TODO: implement handling in communication-protocol/Commands.h
 */
typedef HeaderPackage RelayHeaderPackage;

/**
 * describes a package for master device to origin node comm.: re-synchronize network time
 * TODO: implement handling in communication-protocol/Commands.h
 */
typedef HeaderPackage SyncNetworkTimeHeaderPackage;

/**
 * describes a package that triggers a node reset
 * TODO: implement handling in communication-protocol/Commands.h
 */
typedef HeaderPackage ResetPackage;


/**
 * AckPackage data length expressed as (uint16_t) BufferPointer
 */
#define AckPackagePointerSize HeaderPackagePointerSize

/**
 * describes an acknowledge package with subsequent address
 */
typedef struct AckWithAddressPackage {
    HeaderPackage header;
    uint8_t addressRow : 8;
    uint8_t addressColumn : 8;
} AckWithAddressPackage;

/**
 * PackageHeaderAddress length expressed as (uint16_t) BufferPointer
 */
#define AckWithAddressPackageBufferPointerSize (__pointerBytes(3) | __pointerBits(0))

/**
 * describes an announce network geometry package
 */
typedef struct AnnounceNetworkGeometryPackage {
    HeaderPackage header;
    uint8_t rows : 8;
    uint8_t columns : 8;
} AnnounceNetworkGeometryPackage;

/**
 * AnnounceNetworkGeometryPackage length expressed as (uint16_t) BufferPointer
 */
#define AnnounceNetworkGeometryPackageBufferPointerSize (__pointerBytes(3) | __pointerBits(0))

/**
 * describes a set network geometry package
 */
typedef struct SetNetworkGeometryPackage {
    HeaderPackage header;
    uint8_t rows : 8;
    uint8_t columns : 8;
} SetNetworkGeometryPackage;

/**
 * SetNetworkGeometryPackage length expressed as (uint16_t) BufferPointer
 */
#define SetNetworkGeometryPackageBufferPointerSize (__pointerBytes(3) | __pointerBits(0))

/**
 * describes an enumeration package
 */
typedef struct EnumerationPackage {
    HeaderPackage header;
    uint8_t addressRow : 8;
    uint8_t addressColumn : 8;
    /**
     * Bread crumb is passed until the highest network address is reached.
     */
    uint8_t hasNetworkGeometryDiscoveryBreadCrumb : 1;
} EnumerationPackage;

/**
 * PackageHeaderAddress length expressed as (uint16_t) BufferPointer
 */
#define EnumerationPackageBufferPointerSize (__pointerBytes(3) | __pointerBits(1))

/**
 * Describes a synchronize time package.
 * The package size is chosen to take exactly as long as needed to overflow the timer/counter 1 interrupt.
 * Thus (1+2+2+2+1) * 8 *    (512*2) = 65536 cycles
 *       ^ bytes      ^bits   ^ line coding clock duration in cycles (see configuration/Communication.h)
 */
typedef struct TimePackage {
    HeaderPackage header;
    uint16_t time;
//    uint16_t packageTransmissionLatency;
    uint16_t localTime;
    uint16_t forceUpdateTime : 1;
    uint16_t stuffing1 : 15;
    uint8_t stuffing2;
} TimePackage;

/**
 * PackageHeaderTime length expressed as (uint16_t) BufferPointer
 */
#define TimePackageBufferPointerSize (__pointerBytes(8) | __pointerBits(0))

/**
 * describes a heat wires package
 */
typedef struct HeatWiresPackage {
    HeaderPackage header;
    uint8_t addressRow : 8;
    uint8_t addressColumn: 8;
    uint16_t startTimeStamp : 16;
    uint8_t durationLsb : 8;
    uint8_t durationMsb : 2;
    uint8_t northLeft : 1;
    uint8_t northRight: 1;
    uint8_t __pad: 4;
} HeatWiresPackage;

/**
 * HeatWiresPackage length expressed as (uint16_t) BufferPointer
 */
#define HeatWiresPackageBufferPointerSize (__pointerBytes(6) | __pointerBits(4))

/**
 * describes a heat wires range package
 */
typedef struct HeatWiresRangePackage {
    HeaderPackage header;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
    uint16_t startTimeStamp : 16;
    uint8_t durationLsb : 8;
    uint8_t durationMsb : 2;
    uint8_t northLeft : 1;
    uint8_t northRight: 1;
    uint8_t __pad: 4;
} HeatWiresRangePackage;

/**
 * HeatWiresRangePackage length expressed as (uint16_t) BufferPointer
 */
#define HeatWiresRangePackageBufferPointerSize (__pointerBytes(8) | __pointerBits(4))

/**
 * describes a heat wires mode package
 */
typedef struct HeatWiresModePackage {
    HeaderPackage header;
    uint8_t heatMode : 2;
    uint8_t __pad: 6;
} HeatWiresModePackage;

/**
 * HeatWiresModePackage length expressed as (uint16_t) BufferPointer
 */
#define HeatWiresModePackageBufferPointerSize (__pointerBytes(1) | __pointerBits(2))

/**
 * Union for a convenient way to access buffered packages.
 */
typedef union Package {
    /**
     * package without payload
     */
    HeaderPackage asHeader;
    /**
     * general acknowledge package
     */
    AckPackage asACKPackage;
    /**
     * package transmitted when acknowledging the local address
     */
    AckWithAddressPackage asACKWithLocalAddress;
    /**
     * package received when neighbour acknowledges his address
     */
    AckWithAddressPackage asACKWithRemoteAddress;
    /**
     * package transmitted when enumerating neighbour
     */
    EnumerationPackage asEnumerationPackage;
    /**
     * package transmitted when synchronizing time
     */
    TimePackage asSyncTimePackage;
    /**
     * package transmitted by the topmost, rightmost node when announcing the network geometry
     */
    AnnounceNetworkGeometryPackage asAnnounceNetworkGeometryPackage;
    /**
     * package transmitted by the origin node when setting a new network geometry
     */
    SetNetworkGeometryPackage asSetNetworkGeometryPackage;
    /**
     * package transmitted for scheduling one heat north wires action
     */
    HeatWiresPackage asHeatWiresPackage;
    /**
     * package transmitted for scheduling one heat north wires action in a range of nodes
     */
    HeatWiresRangePackage asHeatWiresRangePackage;
    /**
     * package transmitted to set up the heat wires mode/power
     */
    HeatWiresModePackage asHeatWiresModePackage;
} Package;
