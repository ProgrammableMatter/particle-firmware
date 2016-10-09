/*
 * @author Raoul Rubien 09.10.2016
 *
 * Communication state definition.
 */

#pragma once

#include <stdint.h>


/**
 * facade to bundle port resources
 */
typedef struct DirectionOrientedPort {
    /**
     * discovery related
     */
    DiscoveryPulseCounter *discoveryPulseCounter;
    /**
     * communication related
     */
    TxPort *txPort;
    /**
     * communication related
     */
    RxPort *rxPort;

    /**
     * pointer implementation: decode and interpret reception
     */
    void (*receivePimpl)(void);

    void (*txHighPimpl)(void);

    void (*txLowPimpl)(void);

    /**
     * comm. protocol related
     */
    CommunicationProtocolPortState *protocol;
} DirectionOrientedPort;

/**
 * facade to bundle port resources in a direction oriented way
 */
typedef struct DirectionOrientedPorts {
    /**
     * north port resources
     */
    DirectionOrientedPort north;
    /**
     * east port resources
     */
    DirectionOrientedPort east;
    /**
     * south port resources
     */
    DirectionOrientedPort south;
    /**
     * on simultaneous transmission this port refers to the east port resources
     */
    DirectionOrientedPort simultaneous;
} DirectionOrientedPorts;