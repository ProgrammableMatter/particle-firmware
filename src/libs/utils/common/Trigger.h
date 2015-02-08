// @author: Raoul Rubien 2011

#ifndef _Trigger_h_
#define _Trigger_h_

#include "TriggerMacros.h"


/**
 * trigger macros - generate a ton of trigger function signatures and respective initializations
 * TODO: make it simpler! generate rather simple macros than signatures and functions .... see
 * "toggle" below.
 */
#define MKTRIGGERSIGNATURES(port, pinIn, pinOut) \
void waitForTrigger##port##pinIn##pinOut(void);\

// auto generate signatures
MKTRIGGERSIGNATURES(B, 0, 1);
MKTRIGGERSIGNATURES(B, 1, 2);
MKTRIGGERSIGNATURES(B, 2, 3);
MKTRIGGERSIGNATURES(B, 3, 4);
MKTRIGGERSIGNATURES(B, 4, 5);
MKTRIGGERSIGNATURES(B, 5, 6);
MKTRIGGERSIGNATURES(B, 6, 7);

// since i am lazy and the functions are all void let's redefine them without braces
#define WaitForTriggerB01 \
    waitForTriggerB01()
#define InitTriggerB01 \
// initialize port-pin for input and feedback (output)
TRIGGERINIT(B, 0, 1)

#define WaitForTriggerB12 \
    waitForTriggerB12()
#define InitTriggerB12 \
TRIGGERINIT(B, 1, 2)

#define WaitForTriggerB23 \
    waitForTriggerB23()
#define InitTriggerB23 \
TRIGGERINIT(B, 2, 3)

#define WaitForTriggerB34 \
    waitForTriggerB34()
#define InitTriggerB34 \
TRIGGERINIT(B, 3, 4)

#define WaitForTriggerB45 \
    waitForTriggerB45()
#define InitTriggerB45 \
TRIGGERINIT(B, 4, 5)

#define WaitForTriggerB56 \
    waitForTriggerB56()
#define InitTriggerB56 \
TRIGGERINIT(B, 5, 6)

#define WaitForTriggerB67 \
    waitForTriggerB67()
#define InitTriggerB67 \
TRIGGERINIT(B, 6, 7)


#endif
