//
// Created by Raoul Rubien on 11.05.16.
//

/**
 * definition of all package types that can be transmitted/received
 */
#ifndef __COMMUNICATION_PROTOCOL_TYPES_H__
#define __COMMUNICATION_PROTOCOL_TYPES_H__

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif


/**
 * describes a package header
 */
typedef struct {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
} PackageHeader;

/**
 * PackageHeader data length expressed as BufferPointer
 */
#define PackageHeaderBufferPointerSize ((uint16_t) 0x0001 | ((uint16_t) 1 << (7 + 0)))
/* \
    ((BufferBitPointer) { \
        .byteNumber = 1, \
        .bitMask = (1 << 0) \
    })
*/
/**
 * describes a package header with subsequent address
 */
typedef struct {
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
//#define PackageHeaderAddressBufferPointerSize ((BufferBitPointer) {.byteNumber = 2, .bitMask = ((uint16_t) 1 << 7)})
#define PackageHeaderAddressBufferPointerSize ((uint16_t) 0x0003 | ((uint16_t) 1 << (8 + 0)))
/*\
    ((BufferBitPointer) { \
        .byteNumber = 3, \
        .bitMask = (1 << 0) \
    })
*/
/**
 * describes a package header with subsequent address range
 */
typedef struct {
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
#define PackageHeaderAddressRangeBufferPointerSize ((uint16_t) 0x0005 | ((uint16_t) 1 << (8 + 0)))
/* \
    ((BufferBitPointer){ \
        .byteNumber = 5, \
        .bitMask = (1 << 0) \
    })
*/
/**
 * describes a package header with 7 subsequent data bits
 */
typedef struct {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 7;
} PackageHeaderData7;

/**
 * PackageHeaderData7 length expressed as BufferPointer
 */
#define PackageHeaderData7BufferPointerSize ((uint16_t) 0x0001 | ((uint16_t) 1 << (8 + 6 + 1)))
/* \
    ((BufferBitPointer) { \
        .byteNumber = 1, \
        .bitMask = (1 << (6 + 1)) \
    })
*/

/**
 * describes a package header with 11 subsequent data bits
 */
typedef struct {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 8;
    uint8_t dataMsb : 3;
} PackageHeaderData11;

/**
 * PackageHeaderData11 length expressed as BufferPointer
 */
#define PackageHeaderData11BufferPointerSize ((uint16_t) 0x0002 | ((uint16_t) 1 << (8 + 2 + 1)))
/* \
    ((BufferBitPointer) { \
        .byteNumber = 2, \
        .bitMask = (1 << (2 + 1)) \
    })
*/

/**
 * describes a package header with 19 subsequent data bits
 */
typedef struct {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
} PackageHeaderData19;

/**
 * PackageHeaderData17 length expressed as BufferPointer
 */
#define PackageHeaderData19BufferPointerSize ((uint16_t) 0x0003 | ((uint16_t) 1 << (8 + 2 + 1)))
/* \
    ((BufferBitPointer) { \
        .byteNumber = 3, \
        .bitMask = (1 << (2 + 1)) \
    })
*/

/**
 * describes a package header with subsequent address and seven data bits
 */
typedef struct {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t dataLsb : 7;
} PackageHeaderAddressData7;

/**
 * PackageHeaderAddressData7 length expressed as BufferPointer
 */
#define PackageHeaderAddressData7BufferPointerSize ((uint16_t) 0x003 | ((uint16_t) 1 << (8 + 6 + 1)))
/* \
    ((BufferBitPointer){ \
        .byteNumber = 3, \
        .bitMask = (1 << (6 + 1)) \
    })
*/
/**
 * describes a package header with subsequent address and 11 data bits
 */
typedef struct {
    uint8_t __startBit : 1;
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t dataMsb : 3;
} PackageHeaderAddressData11;

/**
 * PackageHeaderAddressData11 length expressed as BufferPointer
 */
#define PackageHeaderAddressData11BufferPointerSize ((uint16_t) 0x0003 | ((uint16_t) 1 << (8 + 2 + 1)))
/* \
    ((BufferBitPointer){ \
        .byteNumber = 3, \
        .bitMask = (1 << (2 + 1)) \
    })
*/
/**
 * describes a package header with subsequent address and 19 data bits
 */
typedef struct {
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
} PackageHeaderAddressData19;

/**
 * PackageHeaderAddressData19 length expressed as BufferPointer
 */
#define PackageHeaderAddressData19BufferPointerSize ((uint16_t) 0x0004 | ((uint16_t) 1 << (8 + 2 + 1)))
/* \
    ((BufferBitPointer){ \
        .byteNumber = 4, \
        .bitMask = (1 << (2 + 1)) \
    })
*/
/**
 * describes a package header with subsequent address range and 7 data bits
 */
typedef struct {
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
} PackageHeaderAddressRangeData7;

/**
 * PackageHeaderAddressRangeData7 length expressed as BufferPointer
 */
#define PackageHeaderAddressRangeData7BufferPointerSize ((uint16_t) 0x0005 | ((uint16_t) 1 << (8 + 6 + 1)))
/* \
    ((BufferBitPointer){ \
        .byteNumber = 5, \
        .bitMask = (1 << (6 + 1)) \
    })
*/
/**
 * describes a package header with subsequent address range and 11 data bits
 */
typedef struct {
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
} PackageHeaderAddressRangeData11;

/**
 * PackageHeaderAddressRangeData11 length expressed as BufferPointer
 */
#define PackageHeaderAddressRangeData11BufferPointerSize ((uint16_t) 0x0006 | ((uint16_t) 1 << (8 + 2 + 1)))
/*\
    ((BufferBitPointer){ \
        .byteNumber = 6, \
        .bitMask = (1 << (2 + 1)) \
    })
*/
/**
 * describes a package heaer with subsequent address range and 19 data bits
 */
typedef struct {
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
} PackageHeaderAddressRangeData19;

/**
 * PackageHeaderAddressRangeData19 length expressed as BufferPointer
 */
#define PackageHeaderAddressRangeData19BufferPointerSize ((uint16_t) 0x0007 | ((uint16_t) 1 << (8 + 2 + 1)))
/* \
    ((BufferBitPointer){ \
        .byteNumber = 7, \
        .bitMask = ((uint16_t) 1 << (2 + 1)) \
    })
*/
typedef union {
    PackageHeader asHeader;
    PackageHeader asACK;
    PackageHeader asBroadcastHeader;
    PackageHeaderAddress asDedicatedHeader;
    PackageHeaderAddressRange asMulticastHeader;

    PackageHeaderData7 asBroadcastData7;
    PackageHeaderAddressData7 asDedicatedData7;
    PackageHeaderAddressRangeData7 asMulticastData7;

    PackageHeaderData11 asBroadcastData11;
    PackageHeaderAddressData11 asDedicatedData11;
    PackageHeaderAddressRangeData11 asMulticastData11;

    PackageHeaderData19 asBroadcastData19;
    PackageHeaderData19 asACKData19;
    PackageHeaderAddressData19 asDedicatedData19;
    PackageHeaderAddressRangeData19 asMulticastData19;
} Package;


/**
 * describes possible header IDs
 */
typedef enum {
    PACKAGE_HEADER_ID_TYPE_ENUMERATE = 0,
    PACKAGE_HEADER_ID_TYPE_HEAT_WIRES = 1,
    PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE = 2,
    PACKAGE_HEADER_ID_TYPE_FORWARDING_ON = 3,
    PACKAGE_HEADER_ID_TYPE_FORWARDING_OFF = 4,
    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_DISCLOSE = 5,
    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE = 6,
    PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_REQUEST = 7,
    PACKAGE_HEADER_ID_TYPE_RESET = 8,
    PACKAGE_HEADER_ID_TYPE_VERBOSE_TOGGLE = 9,
    PACKAGE_HEADER_ID_TYPE_PING_REQUEST = 10,
    PACKAGE_HEADER_ID_TYPE_PING_RESPONSE = 11,
    PACKAGE_HEADER_ID_TYPE_HEATING_MODE = 12,
    PACKAGE_HEADER_ID_TYPE_ACK = 13,
    PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA = 14,
    PACKAGE_HEADER_ID_TYPE_EXTENDED_HEADER = 15
} PACKAGE_HEADER_ID;

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif

#endif

