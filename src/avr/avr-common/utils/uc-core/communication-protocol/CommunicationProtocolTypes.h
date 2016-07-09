//
// Created by Raoul Rubien on 11.05.16.
//

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
    PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE = 2,
    PACKAGE_HEADER_ID_TYPE_FORWARDING_ON = 3,
    PACKAGE_HEADER_ID_TYPE_FORWARDING_OFF = 4,
    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_DISCLOSE = 5,
    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE = 6,
//    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_REQUEST = 7,
            PACKAGE_HEADER_ID_TYPE_RESET = 8,
    PACKAGE_HEADER_ID_TYPE_SYNC_TIME = 9,
    PACKAGE_HEADER_ID_TYPE_PING_REQUEST = 10,
    PACKAGE_HEADER_ID_TYPE_PING_RESPONSE = 11,
    PACKAGE_HEADER_ID_TYPE_HEATING_MODE = 12,
    PACKAGE_HEADER_ID_TYPE_ACK = 13,
    PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA = 14,
    PACKAGE_HEADER_ID_TYPE_EXTENDED_HEADER = 15
} PackageHeaderId;

/**
 * Describes communication states of the initiator.
 */
typedef enum CommunicationInitiatorStateTypes {
    COMMUNICATION_INITIATOR_STATE_TYPE_IDLE,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED,
    COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED
} CommunicationInitiatorStateTypes;

/**
 * Describes communication states of the receptionist.
 */
typedef enum CommunicationReceptionistStateTypes {
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE
} CommunicationReceptionistStateTypes;

/**
 * describes a package header
 */
typedef struct PackageHeader {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t __pad: 2;
} PackageHeader;

/**
 * PackageHeader data length expressed as BufferPointer
 */
#define PackageHeaderPointerSize \
    ((((uint16_t) 0x0100) << 6) \
    |((uint16_t) 0x0000))

/**
 * describes a package header
 */
typedef struct AckPackage {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t enableBroadcast : 1;
    uint8_t __pad : 1;
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
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t headerIsCommand : 1;
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
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t headerIsCommand : 1;
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
 * describes a package header with subsequent enumeration address and bread crumb flag
 */
typedef struct EnumerationPackage {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
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


/**
 * describes a package header with subsequent address range
 */
typedef struct PackageHeaderAddressRange {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t headerIsCommand : 1;
    uint8_t enableBroadcast : 1;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
} PackageHeaderAddressRange;

/**
 * PackageHeaderAddressRange length expressed as BufferPointer
 */
#define PackageHeaderAddressRangeBufferPointerSize \
    ((((uint16_t) 0x0100) << 0) \
    |((uint16_t) 0x0005))


/**
 * describes a package header with uint16_t subsequent time field
 */
typedef struct TimePackage {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerId : 4;
    uint8_t headerIsCommand : 1;
    uint8_t enableBroadcast : 1;
    uint16_t time : 16;
} TimePackage;

/**
 * PackageHeaderTime length expressed as BufferPointer
 */
#define TimePackageBufferPointerSize  \
    ((((uint16_t) 0x0100) << 0) \
    |((uint16_t) 0x0003))

/**
 * union for a convenient way to access buffered packages
 */
typedef union Package {
    PackageHeader asHeader;
    AckPackage asACKPackage;
    AckWithAddressPackage asACKWithLocalAddress;
    AckWithAddressPackage asACKWithRemoteAddress;
    EnumerationPackage asEnumerationPackage;
    TimePackage asSyncTimePackage;
    AnnounceNetworkGeometryPackage asAnnounceNetworkGeometryPackage;
} Package;

/**
 * describes the communication port state
 */
typedef struct CommunicationProtocolPortState {
    CommunicationInitiatorStateTypes initiatorState;
    CommunicationReceptionistStateTypes receptionistState;
    /**
     * value of 0 indicates timeout
     */
    uint8_t stateTimeoutCounter;
} CommunicationProtocolPortState;

typedef struct CommunicationProtocolPorts {
    CommunicationProtocolPortState north;
    CommunicationProtocolPortState east;
    CommunicationProtocolPortState south;
} CommunicationProtocolPorts;

/**
 * describes the network geometry; valid row/col values are > 0
 */
typedef struct NetworkGeometry {
    uint8_t rows;
    uint8_t columns;
} NetworkGeometry;

/**
 * describes the communication ports
 */
typedef struct CommunicationProtocol {
    CommunicationProtocolPorts ports;
    NetworkGeometry networkGeometry;
    uint8_t hasNetworkGeometryDiscoveryBreadCrumb : 1;
    uint8_t isBroadcastEnabled : 1;
    uint8_t isSimultaneousTransmissionEnabled : 1;
    uint8_t __pad : 5;
} CommunicationProtocol;