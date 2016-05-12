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
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t __pad : 4;
} PackageHeader;

typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t row0 : 8;
    uint8_t column1 : 8;
    uint8_t __pad : 4;
} PackageHeaderAddress;

typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t row0 : 8;
    uint8_t column0 : 8;
    uint8_t row1 : 8;
    uint8_t column1 : 8;
    uint8_t __pad : 4;
} PackageHeaderAddressrange;


typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t dataLsb : 7;
    uint8_t __pad : 5;
} PackageHeaderData7;

typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t dataLsb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 1;
} PackageHeaderData11;

typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 1;
} PackageHeaderData19;

typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t row0 : 8;
    uint8_t column1 : 8;
    uint8_t dataLsb : 7;
    uint8_t __pad : 5;
} PackageHeaderAddressData7;

typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t row0 : 8;
    uint8_t column1 : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 1;
} PackageHeaderAddressData11;

typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t row0 : 8;
    uint8_t column1 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 1;
} PackageHeaderAddressData19;

typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t row0 : 8;
    uint8_t column0 : 8;
    uint8_t row1 : 8;
    uint8_t column1 : 8;
    uint8_t dataLsb : 7;
    uint8_t __pad : 5;
} PackageHeaderAddressrangeData7;


typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t row0 : 8;
    uint8_t column0 : 8;
    uint8_t row1 : 8;
    uint8_t column1 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 1;
} PackageHeaderAddressrangeData11;


typedef struct {
    uint8_t isStream : 1;
    uint8_t isCommand : 1;
    uint8_t isBroadcast : 1;
    uint8_t reserved : 1;
    uint8_t row0 : 8;
    uint8_t column0 : 8;
    uint8_t row1 : 8;
    uint8_t column1 : 8;
    uint8_t dataLsb : 8;
    uint8_t dataCeb : 8;
    uint8_t dataMsb : 3;
    uint8_t __pad : 1;
} PackageHeaderAddressrangeData19;

typedef union {
    PackageHeader asBroadcastHeader;
    PackageHeaderAddress asHader;
    PackageHeaderAddressrange asMulticastHeader;

    PackageHeaderData7 asBroadcastData7;
    PackageHeaderData11 asBroadcastData11;
    PackageHeaderData19 asBroadcastData19;

    PackageHeaderAddressData7 asData7;
    PackageHeaderAddressData11 asData11;
    PackageHeaderAddressData19 asData19;

    PackageHeaderAddressrangeData7 asMulticastData7;
    PackageHeaderAddressrangeData11 asMulticastData11;
    PackageHeaderAddressrangeData19 asMulticastData19;
} Package;

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif

#endif
