/**
 * @author Raoul Rubien 2016
 */

#ifndef COMMUNICATION_TYPES_H
#define COMMUNICATION_TYPES_H

#include <inttypes.h>

/**
 * Describes a bit within a 4 byte buffer.
 */
typedef struct {
    unsigned char byteNumber : 2; // byte number
    unsigned char bitMask; // the bit in the byte
} BufferBitPointer;


/**
 * Provides a linear 4 byte buffer and a bit pointer per communication channel.
 * The struct is used for transmission and reception as well.
 */

typedef struct {
    unsigned char bytes[4];
    BufferBitPointer pointer;
    unsigned char isEmpty : 1;
} PortBuffer;

typedef struct {
    PortBuffer north;
    PortBuffer south;
    PortBuffer east;
} BitBuffer;

typedef BitBuffer TxBitBuffer;
typedef BitBuffer RxBitBuffer;


/**
 * Keeps the channel's synchronization offset and a short reception counter;
 */
typedef struct {
    uint8_t isReceiving : 2; // is decremented on each expected coding flank, set to top on reception
    uint16_t counterOffset; // synchronization offset from fist received bit relative to compare counter

} ReceptionCounterChannelParameters;
/**
 * Reception de-coding arguments for all channels.
 */
typedef struct {
    uint16_t top; // the max value the counter 1 is counting to
    uint16_t center; // usually 1/2 of max value
    uint16_t receptionDelta; // allowed reception deviation of center / top
    uint16_t receptionOffset; // the synchronization offset according to the common counter
    uint16_t leftOfTop; // minimum reception counter at top
    uint16_t rightOfTop; // maximum reception counter at top
    uint16_t leftOfCenter; // minimum reception counter at center
    uint16_t rightOfCenter; // maximum reception counter at center
    uint16_t syncCheck; // when to check if bit was received or transmission terminated
    ReceptionCounterChannelParameters north;
    ReceptionCounterChannelParameters east;
    ReceptionCounterChannelParameters south;
} ReceptionCounterParameters;

#endif