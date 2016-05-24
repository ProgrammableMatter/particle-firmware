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

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
} PackageHeader;

#define PackageHeaderBufferPointerSize ((BufferBitPointer) {.byteNumber = 0, .bitMask = 0x80})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
} PackageHeaderAddress;

#define PackageHeaderAddressBufferPointerSize ((BufferBitPointer) {.byteNumber = 2, .bitMask = 0x80})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
} PackageHeaderAddressrange;

#define PackageHeaderAddressrangeBufferPointerSize ((BufferBitPointer){.byteNumber = 4, .bitMask = 0x80})


typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 7;
} PackageHeaderData7;

#define PackageHeaderData7BufferPointerSize ((BufferBitPointer){.byteNumber = 1, .bitMask = 0x40})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 8;
    uint8_t dataMsb : 3;
} PackageHeaderData11;

#define PackageHeaderData11BufferPointerSize ((BufferBitPointer) {.byteNumber = 2, .bitMask = 0x04})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
} PackageHeaderData19;

#define PackageHeaderData19BufferPointerSize ((BufferBitPointer) {.byteNumber = 3, .bitMask = 0x04})


typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t dataLsb : 7;
} PackageHeaderAddressData7;

#define PackageHeaderAddressData7BufferPointerSize ((BufferBitPointer){.byteNumber = 3, .bitMask = 0x40})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t dataMsb : 3;
} PackageHeaderAddressData11;

#define PackageHeaderAddressData11BufferPointerSize ((BufferBitPointer){.byteNumber = 3, .bitMask = 0x04})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
} PackageHeaderAddressData19;

#define PackageHeaderAddressData19BufferPointerSize ((BufferBitPointer){.byteNumber = 4, .bitMask = 0x04})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
    uint8_t dataLsb : 7;
} PackageHeaderAddressrangeData7;

#define PackageHeaderAddressrangeData7BufferPointerSize ((BufferBitPointer){.byteNumber = 5, .bitMask = 0x40})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataMsb : 3;
} PackageHeaderAddressrangeData11;

#define PackageHeaderAddressrangeData11BufferPointerSize ((BufferBitPointer){.byteNumber = 6, .bitMask = 0x04})

typedef struct {
    uint8_t headerIsStream : 1;
    uint8_t headerIsCommand : 1;
    uint8_t headerIsBroadcast : 1;
    uint8_t headerIsReserved : 1;
    uint8_t headerId : 4;
    uint8_t addressRow0 : 8;
    uint8_t addressColumn0 : 8;
    uint8_t addressRow1 : 8;
    uint8_t addressColumn1 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
} PackageHeaderAddressrangeData19;

#define PackageHeaderAddressrangeData19BufferPointerSize ((BufferBitPointer){.byteNumber = 7, .bitMask = 0x04})

typedef union {
    PackageHeader asBroadcastHeader;
    PackageHeaderAddress asDedicatedHeader;
    PackageHeaderAddressrange asMulticastHeader;

    PackageHeaderData7 asBroadcastData7;
    PackageHeaderData11 asBroadcastData11;
    PackageHeaderData19 asBroadcastData19;

    PackageHeaderAddressData7 asDedicatedData7;
    PackageHeaderAddressData11 asDedicatedData11;
    PackageHeaderAddressData19 asDedicatedData19;

    PackageHeaderAddressrangeData7 asMulticastData7;
    PackageHeaderAddressrangeData11 asMulticastData11;
    PackageHeaderAddressrangeData19 asMulticastData19;
} Package;

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif

#endif

