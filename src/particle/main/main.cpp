/**
 * @author Raoul Rubien 2015
 */
#include <avr/interrupt.h>

#include <common/common.h>
extern "C" {
#include "common/pure_virtual_pita.h"
}
;
#include "PortDefinition.h"
#include "uc-core/Particle.h"

#include "fuses.h"

/**
 * free function declarations
 */
void init(void);

int main(void) {
	Particle core;
	uint8_t bytes[10];
	uint8_t inBits;

	Indicator indicator;
	indicator.enterLedState(Indicator::ERROR);

	forever {
		core.readBits(ParticleCore::A, bytes, 10, &inBits);
		core.writeBits(ParticleCore::A, bytes, 10);
		core.tick();
		indicator.tick();
		// TODO implement indicator && core
		adf
	}
}

/**
 * initialization before main is called
 */
void init() {
}

