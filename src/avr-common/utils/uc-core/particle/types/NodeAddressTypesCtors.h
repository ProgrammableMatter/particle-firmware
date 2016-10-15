/*
 * @author Raoul Rubien 2016
 *
 * Node address types constructor implementation.
 */

#pragma once

#include "NodeAddressTypes.h"

/**
 * constructor function
 * @param o the object to construct
 */
void constructNodeAddress(NodeAddress *const o) {
    o->row = 0;
    o->column = 0;
}
