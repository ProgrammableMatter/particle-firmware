/*
 * @author Raoul Rubien 2016
 *
 * Node address state definition.
 */

#pragma once

#include <stdint.h>

/**
 * The node address in the network. The origin node is addressed as row=1, column=1.
 * Address (0,0) is reserved.
 */
typedef struct NodeAddress {
    uint8_t row;
    uint8_t column;
} NodeAddress;
