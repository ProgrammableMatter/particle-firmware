/**
 * @author Raoul Rubien 02.10.2016
 *
 * Calculation related types.
 */

#pragma once

#include <stdint.h>

#ifdef C_STRUCTS_TO_JSON_PARSER_TYPEDEF_NOT_SUPPORTED_SUPPRESS_REGULAR_TYPEDEFS
#  define CumulationType  uint32_t
#  define SampleValueType uint16_t
#  define CalculationType float
#  define IndexType uint8_t
#else
typedef uint32_t CumulationType;
typedef uint16_t SampleValueType;
typedef float CalculationType;
typedef uint8_t IndexType;
#endif
