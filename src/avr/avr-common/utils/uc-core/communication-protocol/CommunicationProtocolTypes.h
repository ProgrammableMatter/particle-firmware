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
    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_REQUEST = 7,
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
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
} PackageHeader;

/**
 * PackageHeader data length expressed as BufferPointer
 */
#define PackageHeaderBufferPointerSize \
    ((((uint16_t) 0x0100) << 0) \
    |((uint16_t) 0x0001))

/**
 * describes a package header with subsequent address
 */
typedef struct PackageHeaderAddress {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
} PackageHeaderAddress;

/**
 * PackageHeaderAddress length expressed as BufferPointer
 */
#define PackageHeaderAddressBufferPointerSize \
    ((((uint16_t) 0x0100) << 0)\
    |((uint16_t) 0x0003))

/**
 * describes a package header with subsequent enumeration address and bread crumb flag
 */
typedef struct PackageHeaderEnumeration {
    uint8_t __startBit : 1;
    uint8_t headerIsBreadCrumb : 1;
    uint8_t __reserved0 : 1;
    uint8_t __reserved1 : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
} PackageHeaderEnumeration;

/**
 * PackageHeaderAddress length expressed as BufferPointer
 */
#define PackageHeaderEnumerationBufferPointerSize \
    ((((uint16_t) 0x0100) << 0)\
    |((uint16_t) 0x0003))


/**
 * describes a package header with subsequent address range
 */
typedef struct PackageHeaderAddressRange {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
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
 * describes a package header with 7 subsequent data bits
 */
typedef struct PackageHeaderData7 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 7;
    uint8_t __pad : 1;
} PackageHeaderData7;

/**
 * PackageHeaderData7 length expressed as BufferPointer
 */
#define PackageHeaderData7BufferPointerSize  \
    ((((uint16_t) 0x0100) << 7) \
    |((uint16_t) 0x0001))

/**
 * describes a package header with 11 subsequent data bits
 */
typedef struct PackageHeaderData11 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 5;
} PackageHeaderData11;

/**
 * PackageHeaderData11 length expressed as BufferPointer
 */
#define PackageHeaderData11BufferPointerSize  \
    ((((uint16_t) 0x0100) << 3) \
    |((uint16_t) 0x0002))


/**
 * describes a package header with uint16_t subsequent time field
 */
typedef struct PackageHeaderTime {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint16_t time : 16;
} PackageHeaderTime;

/**
 * PackageHeaderTime length expressed as BufferPointer
 */
#define PackageHeaderTimeBufferPointerSize  \
    ((((uint16_t) 0x0100) << 0) \
    |((uint16_t) 0x0003))

/**
 * describes a package header with 19 subsequent data bits
 */
typedef struct PackageHeaderData19 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 5;
} PackageHeaderData19;

/**
 * PackageHeaderData19 length expressed as BufferPointer
 */
#define PackageHeaderData19BufferPointerSize  \
    ((((uint16_t) 0x0100) << 3) \
    |((uint16_t) 0x0003))

/**
 * describes a package header with subsequent address and seven data bits
 */
typedef struct PackageHeaderAddressData7 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t dataLsb : 7;
    uint8_t __pad : 1;
} PackageHeaderAddressData7;

/**
 * PackageHeaderAddressData7 length expressed as BufferPointer
 */
#define PackageHeaderAddressData7BufferPointerSize  \
    ((((uint16_t) 0x0100) << 7) \
    |((uint16_t) 0x0003))

/**
 * describes a package header with subsequent address and 11 data bits
 */
typedef struct PackageHeaderAddressData11 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 5;
} PackageHeaderAddressData11;

/**
 * PackageHeaderAddressData11 length expressed as BufferPointer
 */
#define PackageHeaderAddressData11BufferPointerSize   \
    ((((uint16_t) 0x0100) << 3) \
    |((uint16_t) 0x0003))

/**
 * describes a package header with subsequent address and 19 data bits
 */
typedef struct PackageHeaderAddressData19 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 5;
} PackageHeaderAddressData19;

/**
 * PackageHeaderAddressData19 length expressed as BufferPointer
 */
#define PackageHeaderAddressData19BufferPointerSize   \
    ((((uint16_t) 0x0100) << 3) \
    |((uint16_t) 0x0004))

/**
 * describes a package header with subsequent address range and 7 data bits
 */
typedef struct PackageHeaderAddressRangeData7 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
    uint8_t dataLsb : 7;
    uint8_t __pad : 1;
} PackageHeaderAddressRangeData7;

/**
 * PackageHeaderAddressRangeData7 length expressed as BufferPointer
 */
#define PackageHeaderAddressRangeData7BufferPointerSize  \
    ((((uint16_t) 0x0100) <<  7) \
    |((uint16_t) 0x0005))

/**
 * describes a package header with subsequent address range and 11 data bits
 */
typedef struct PackageHeaderAddressRangeData11 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 5;
} PackageHeaderAddressRangeData11;

/**
 * PackageHeaderAddressRangeData11 length expressed as BufferPointer
 */
#define PackageHeaderAddressRangeData11BufferPointerSize   \
    ((((uint16_t) 0x0100) << 3 \
    |((uint16_t) 0x0006))

/**
 * describes a package header with subsequent address range and 19 data bits
 */
typedef struct PackageHeaderAddressRangeData19 {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 5;
} PackageHeaderAddressRangeData19;

/**
 * PackageHeaderAddressRangeData19 length expressed as BufferPointer
 */
#define PackageHeaderAddressRangeData19BufferPointerSize  \
    ((((uint16_t) 0x0100) << 3) \
    |((uint16_t) 0x0007))

/**
 * union for a convenient way to access buffered packages
 */
typedef union Package {
    PackageHeader asHeader;
    PackageHeader asACKPackage;
    PackageHeader asBroadcastHeader;

    PackageHeaderAddress asACKWithLocalAddress;
    PackageHeaderAddress asACKWithRemoteAddress;
    PackageHeaderEnumeration asEnumerationPackage;
    PackageHeaderTime asSyncTimePackage;
    PackageHeaderAddressRange asMulticastHeader;

    PackageHeaderData7 asBroadcastData7;
    PackageHeaderAddressData7 asDedicatedData7;
    PackageHeaderAddressRangeData7 asMulticastData7;

    PackageHeaderData11 asBroadcastData11;
    PackageHeaderAddressData11 asDedicatedData11;
    PackageHeaderAddressRangeData11 asMulticastData11;

    PackageHeaderData19 asBroadcastData19;
    PackageHeaderAddressData19 asDedicatedData19;
    PackageHeaderAddressRangeData19 asMulticastData19;
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
 * describes the communication ports
 */
typedef struct CommunicationProtocol {
    CommunicationProtocolPorts ports;
} CommunicationProtocol;