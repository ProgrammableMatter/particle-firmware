/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related arguments.
 */

#pragma once

#include "uc-core/configuration/Evaluation.h"

#ifdef EVALUATION_SYNC_CYCLICALLY
#  define SYNCHRONIZATION_TYPES_CTORS_FIRST_SYNC_PACKAGE_LOCAL_TIME ((uint16_t) 160)
#  define SYNCHRONIZATION_TYPES_CTORS_FAST_SYNC_PACKAGE_SEPARATION ((uint8_t) 20)
#  define SYNCHRONIZATION_TYPES_CTORS_SYNC_PACKAGE_SEPARATION ((uint16_t) 340)
#  define SYNCHRONIZATION_TYPES_CTORS_TOTAL_FAST_SYNC_PACKAGES ((uint16_t) 5)
#endif

#ifdef EVALUATION_SIMPLE_SYNC_AND_ACTUATION
#  define SYNCHRONIZATION_TYPES_CTORS_FIRST_SYNC_PACKAGE_LOCAL_TIME ((uint16_t) 350)
#  define SYNCHRONIZATION_TYPES_CTORS_FAST_SYNC_PACKAGE_SEPARATION ((uint8_t) 40)
#  define SYNCHRONIZATION_TYPES_CTORS_SYNC_PACKAGE_SEPARATION ((uint16_t) 80)
#  define SYNCHRONIZATION_TYPES_CTORS_TOTAL_FAST_SYNC_PACKAGES ((uint16_t) 30)
#endif

#ifdef EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG
#  define SYNCHRONIZATION_TYPES_CTORS_FAST_SYNC_PACKAGE_SEPARATION ((uint16_t) 64)
#  define SYNCHRONIZATION_TYPES_CTORS_SYNC_PACKAGE_SEPARATION ((uint16_t) 64)
//#  define SYNCHRONIZATION_TYPES_CTORS_SYNC_PACKAGE_SEPARATION ((uint16_t) 300)
//#  define SYNCHRONIZATION_TYPES_CTORS_TOTAL_FAST_SYNC_PACKAGES ((uint16_t) 50)
#  define SYNCHRONIZATION_TYPES_CTORS_TOTAL_FAST_SYNC_PACKAGES ((uint16_t) 5)
#endif

#ifdef EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG_IN_PHASE_SHIFTING
#  define SYNCHRONIZATION_TYPES_CTORS_FAST_SYNC_PACKAGE_SEPARATION ((uint16_t) 64)
#  define SYNCHRONIZATION_TYPES_CTORS_SYNC_PACKAGE_SEPARATION ((uint16_t) 64)
#  define SYNCHRONIZATION_TYPES_CTORS_TOTAL_FAST_SYNC_PACKAGES ((uint16_t) 5)
#endif


#ifdef EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG_THEN_ACTUATE_ONCE
#  define SYNCHRONIZATION_TYPES_CTORS_FIRST_SYNC_PACKAGE_LOCAL_TIME ((uint16_t) 350)
#  define SYNCHRONIZATION_TYPES_CTORS_FAST_SYNC_PACKAGE_SEPARATION ((uint16_t) 40)
#  define SYNCHRONIZATION_TYPES_CTORS_SYNC_PACKAGE_SEPARATION ((uint16_t) 80)
#  define SYNCHRONIZATION_TYPES_CTORS_TOTAL_FAST_SYNC_PACKAGES ((uint16_t) 30)
#endif