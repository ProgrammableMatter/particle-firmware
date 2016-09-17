/**
 * @author Raoul Rubien 2015
 */

#pragma once

#include "uc-core/configuration/Discovery.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/particle/Globals.h"

/**
 * Increments the port discovery counter, but Overflows at RX_DISCOVERY_PULSE_COUNTER_MAX.
 * @param portCounter reference to the designated port counter
 */
extern FUNC_ATTRS void dispatchFallingDiscoveryEdge(volatile DiscoveryPulseCounter *portCounter);

FUNC_ATTRS void dispatchFallingDiscoveryEdge(volatile DiscoveryPulseCounter *portCounter) {
    if (portCounter->counter < RX_DISCOVERY_PULSE_COUNTER_MAX) {
        portCounter->counter++;

        if (portCounter == ParticleAttributes.directionOrientedPorts.north.discoveryPulseCounter) {
            LED_STATUS1_TOGGLE;
        }
    } else {
        portCounter->isConnected = true;
//        if (portCounter == ParticleAttributes.directionOrientedPorts.north.discoveryPulseCounter) {
//            LED_STATUS1_TOGGLE;
//        }
    }
}

/**
 * Updates the node type according to the amount of incoming pulses.
 * The type {@link NodeType} is stored to the {@link ParticleAttributes.type} field.
 * @return true if the node is fully connected, false otherwise
 */
extern FUNC_ATTRS bool updateAndDetermineNodeType(void);

FUNC_ATTRS bool updateAndDetermineNodeType(void) {
    if (ParticleAttributes.discoveryPulseCounters.north.isConnected) { // N
        if (ParticleAttributes.discoveryPulseCounters.south.isConnected) { // N, S
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) { // N, S, E
                ParticleAttributes.node.type = NODE_TYPE_INTER_HEAD;
                return true;
            } else { // N,S,!E
                ParticleAttributes.node.type = NODE_TYPE_INTER_NODE;
            }
        } else { // N, !S
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) { // N, !S, E
                ParticleAttributes.node.type = NODE_TYPE_INTER_HEAD;
            } else { // N, !S, !E
                ParticleAttributes.node.type = NODE_TYPE_TAIL;
            }
        }
    } else { // !N
        if (ParticleAttributes.discoveryPulseCounters.south.isConnected) { // !N, S
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) { // !N, S, E
                ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
            } else { // !N, S, !E
                ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
            }
        } else { // !N, !S
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) { // !N, !S, E
                ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
            } else { // !N, !S, !E
                ParticleAttributes.node.type = NODE_TYPE_ORPHAN;
            }
        }
    }
    return false;
}
