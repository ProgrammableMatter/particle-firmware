/**
 * @author Raoul Rubien 2015
 */
#include <avr/interrupt.h>

#include <common/common.h>
#include <common/PortADefinition.h>

extern "C" {};

/**
 * free function declarations
 */
void init(void);


int main(void) {
	// enable signal led
    ADir setOut Pin3;
    AOut setHi Pin3;

    // set test point high
    ADir setOut Pin7;
    AOut setHi Pin7;

    // rx sense B
    //ADir setIn  Pin2;
    //AOut pullUp Pin2;
    // rx sense A
    //ADir setIn  Pin5;
    //AOut pullUp Pin5;

    // pull rx-B mosfet to Vcc
    ADir setOut Pin0;
    AOut setLo Pin0;


    // pull rx-A mosfet to Vcc
    ADir setOut Pin6;
    AOut setLo Pin6;

    // pull tx-B mosfet to Vcc
    ADir setOut Pin1;
    AOut setHi Pin1;

    // pull tx-A mosfet to Vcc
    ADir setOut Pin4;
    AOut setHi Pin4;
    forever{
    }
}

/**
 * initialization before main is called
 */
void init() {
}

