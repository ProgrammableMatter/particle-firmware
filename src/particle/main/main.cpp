/**
 * @author Raoul Rubien 2015
 */
#include <avr/interrupt.h>

#include <common/common.h>
#include "PortDefinition.h"

#include "fuses.h"

extern "C" {

}
;

/**
 * free function declaration
 */
void init(void);

int main(void) {
	forever {
		// signal led
		ADir setOut Pin3;
		AOut setHi Pin3;

		// signal tp
		ADir setOut Pin7;
		AOut setHi Pin7;
		/*
		 // rx sense B
		 ADir setIn  Pin2;
		 AOut pullUp Pin2;
		 // rx sense A
		 ADir setIn  Pin5;
		 AOut pullUp Pin5;
		 */
		// sw / rx-A
		ADir setOut Pin0;
		AOut setLo Pin0;
		// sw / rx-A
		ADir setOut Pin6;
		AOut setLo Pin6;

		// tx-B
		ADir setOut Pin1;
		AOut setHi Pin1;

		// tx-A
		ADir setOut Pin4;
		AOut setHi Pin4;

	}
}

/**
 * initialization before main is called
 */
void init() {
}

