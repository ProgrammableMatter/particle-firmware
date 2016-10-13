/**
 * @author Raoul Rubien 3.10.2016
 *
 * Communication related arguments.
 */

#pragma once

/**
 * If defined enables merging the high bits [15:1] of the time stamp with
 * the edge direction bit to one uint8_t which uses 1/3 less memory for
 * decoding buffer but looses the least significant bit for timestamp accuracy.
 */
//#define MANCHESTER_DECODING_ENABLE_MERGE_TIMESTAMP_WITH_EDGE_DIRECTION

/**
 * Size of reception snapshot buffer per port.
 */
// 88% snapshots buffer of 9 byte PDU max. events
//#define MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS 127
// 44% snapshots buffer of max. 9 byte PDU max. events
//#define MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS 64
// 22% snapshots buffer of 9 byte PDU max. events
//#define MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS 32
// 20% snapshots buffer of 9 byte PDU max. events
#define MANCHESTER_DECODING_RX_NUMBER_SNAPSHOTS 40

