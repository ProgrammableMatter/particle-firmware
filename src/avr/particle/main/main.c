/**
 * @author Raoul Rubien 2015
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <common/common.h>

//#include "uc-core/Indicator.h"
#include <uc-core/ParticleIoDefinitions.h>
#include <uc-core/Particle.h>


/**
 * set "void init()" as initialization function
 */
//void init(void) __attribute__ ((naked)) __attribute__ ((section (".init3")));


void init(void);
/**
 * the main execution loop
 */
int main(void) {
    init();
    forever {
        particle_tick();
    }
}

/**
 * initialize ports and interrupts before {@link #main(void)}
 */
void init(void) {
    cli();
    // init clock
    CCP = 0xD8;           // unlock CLKSMR
    CLKMSR = 0b00;      // configure internal clock, 8MHz
    CCP = 0xD8;           // unlock CLKPSR
    CLKPSR = 0b0000;    // set clock prescale 1

    // init conter 0 compare
    //#ifdef __AVR_ATtiny20__
// disconnect interrupt from port
    //TCCR0A unsetBit bit (COM0A1);
    //TCCR0A unsetBit bit (COM0A0);

// set waveform generation to normal
    //TCCR0A unsetBit bit (WGM00);
    //TCCR0A unsetBit bit (WGM01);
    //TCCR0B unsetBit bit (WGM02);

// set timer prescaler
    TCCR0B setBit (bit(CS01) | bit(CS00));

    //TCNT0 = 0x00; // clear counter
    //OCR0A = 0x10; // set compare value
// enable interrupt
    //TIMSK setBit bit(OCIE0A);
    TIMSK setBit bit(TOIE0);
//#endif


//    Indicator();

    LED_SETUP;
    TESTPOINT_SETUP;
    NORTH_TX_SETUP;
    NORTH_RX_SETUP;
    NORTH_RX_SWITCH_SETUP;
    SOUTH_TX_SETUP;
    SOUTH_RX_SETUP;
    SOUTH_RX_SWITCH_SETUP;
    sei();
}






