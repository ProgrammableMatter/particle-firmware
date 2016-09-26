/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related types.
 */

#pragma once

#ifdef C_STRUCTS_TO_JSON_PARSER_TYPEDEF_NOT_SUPPORTED_SUPPRESS_REGULAR_TYPEDEFS
# define CumulationType uint32_t
# define SampleValueType uint16_t
# define CalculationType float
#define IndexType uint8_t
#else
typedef uint32_t CumulationType;
typedef uint16_t SampleValueType;
typedef float CalculationType;
typedef uint8_t IndexType;
#endif

typedef struct LeastSquareRegressionResult {
    /**
    * k as known of f(x)=k*x+d
    */
    CalculationType k;
    /**
    * d as known of f(x)=k*x+d
    */
    CalculationType d;

    /**
    * Indicates whether the fields for calculation results are valid or not.
    */
    uint8_t isCalculationValid : 1;
    uint8_t __pad : 7;

} LeastSquareRegressionResult;