/**
 * @author Raoul Rubien 2015
 */

#include <uc-core/particle/ParticleLoop.h>

FUSES = {
        .low = LFUSE_DEFAULT,
        .high = HFUSE_DEFAULT,
        .extended = EFUSE_DEFAULT,
};

int main(void) {
    processLoop();
    return 0;
}



