/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include "uc-core/configuration/Discovery.h"


extern FUNC_ATTRS void dispatchFallingDiscoveryEdge(volatile DiscoveryPulseCounter *portCounter);
/**
 * increments the port counter
 */
FUNC_ATTRS void dispatchFallingDiscoveryEdge(volatile DiscoveryPulseCounter *portCounter) {
    if (portCounter->counter < RX_DISCOVERY_PULSE_COUNTER_MAX) {
        portCounter->counter++;
    } else {
        portCounter->isConnected = true;
    }
}

extern FUNC_ATTRS bool updateAndDetermineNodeType(void);
/**
 * Updates the node type according to the amount of incoming pulses. The type {@link NodeType} is stored to
 * {@link ParticleAttributes.type}.
 * @return true if the node is fully connected, else false
 */
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
