//
// Created by Raoul Rubien on 25.05.16.
//

#pragma once

#include "InterpreterTypes.h"
#include "CommunicationProtocolTypes.h"

#ifdef TRY_INLINE
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif

/**
 * interpret the interpreter buffer according to the particle state as context
 */
FUNC_ATTRS void interpretRxBuffer(volatile RxPort *rxPort) {

    DEBUG_CHAR_OUT('I');
    if (!isNotReceiving(rxPort)) {
        DEBUG_CHAR_OUT('i');
        return;
    }

    Package *package = (Package *) rxPort->buffer.bytes;
    switch (ParticleAttributes.node.state) { // switch according to node state context

        // state: wait for being enumerated
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            // expect enumeration package with new address to assign locally
            switch (package->asEnumerationPackage.headerId) {
                case PACKAGE_HEADER_ID_TYPE_ENUMERATE:
                    if (equalsPackageSize(rxPort->buffer.pointer, PackageHeaderAddressBufferPointerSize)) {
                        DEBUG_CHAR_OUT('E');
                        ParticleAttributes.node.address.row = package->asEnumerationPackage.addressRow0;
                        ParticleAttributes.node.address.column = package->asEnumerationPackage.addressColumn0;
                        ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_SEND_ACK_RESPONSE_TO_PARENT;
                    }
                    clearReceptionBuffer(rxPort);

                    break;
                default:
                    DEBUG_CHAR_OUT('e');
                    // otherwise remain in same state
                    break;
            }
            break;

            // state: wait for ack response from parent to finish enumeration
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_ACK_RESPONSE_FROM_PARENT:
            // expect ack package from parent, otherwise switch back to waiting for enumeration state
            switch (package->asACKPackage.headerId) {
                case PACKAGE_HEADER_ID_TYPE_ACK:
//              xxx
                    if (equalsPackageSize(rxPort->buffer.pointer, PackageHeaderBufferPointerSize)) {
                        clearReceptionBuffer(rxPort);
                        DEBUG_CHAR_OUT('K');
                        // data ok, switch to next state
                        ParticleAttributes.node.state = STATE_TYPE_LOCALLY_ENUMERATED;
                    }
                    break;

                case PACKAGE_HEADER_ID_TYPE_ENUMERATE:
//                                      xxx
                    // on address reassignment, do not clear buffer but switch state
                    ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
                    DEBUG_CHAR_OUT('W');
                    break;
                default:
//                    // on any other package, clear buffer and switch state
//                    clearReceptionBuffer(rxPort);
//                    ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
//                    DEBUG_CHAR_OUT('w');
                    break;
            }
            break;

            // state: wait for ack response with 2 byte data describing neighbour's address
        case STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_FROM_SOUTH:
            // if data is ok, then switch to next state, otherwise re-start enumeration
            switch (package->asACKWithLocalAddress.headerId) {
//                                  xxx
                case PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA:
                    DEBUG_CHAR_OUT('T');

                    if (equalsPackageSize(rxPort->buffer.pointer, PackageHeaderData19BufferPointerSize)) {
                        // data ok, switch to next state
                        ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_SEND_ACK_RESPONSE_TO_SOUTH;
                        clearReceptionBuffer(rxPort);
                    }
                    break;
                default:
//                    // otherwise re-start the enumeration
//                    DEBUG_CHAR_OUT('t');
//                    clearReceptionBuffer(rxPort);
//                    ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR;
                    break;
            }
            break;
        default:
            break;
    }

    DEBUG_CHAR_OUT('i');
}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif
