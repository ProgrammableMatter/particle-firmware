/**
 * @author Raoul Rubien 2015
 */

#include <uc-core/particle/ParticleLoop.h>

FUSES = {
        .low =  (FUSE_SUT_CKSEL0 & FUSE_SUT_CKSEL2 & FUSE_SUT_CKSEL3 & FUSE_SUT_CKSEL4 & FUSE_CKOUT),
        .high = HFUSE_DEFAULT,
        .extended = EFUSE_DEFAULT,
};

int main(void) {
    processLoop();
    return 0;
}



