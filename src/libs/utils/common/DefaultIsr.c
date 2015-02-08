// @author: Raoul Rubien 2011

#include <avr/io.h>

#include <avr/interrupt.h>

#include "usart/Usart.h"
#include "IntTypes.h"


/**
 * previous interrupt had no vector
 */
//ISR( BADISR_vect ) {
//    printf("\nInterrupt: BADISR_vect: ");
//    UCSR0A &= ~(RXC0);  //clear rx complete bit
//    // check if we have framing overrun or parity error
//    if ( UCSR0A & ( _BV(FE0) | _BV(DOR0) | _BV(UPE0)) ) {
//        printf("\nUSART0 framing error %s:%d\n",  __FILE__, __LINE__);
//        forever;
//    }
//    else {
//        printf("\nunknown Interrupt: %s:%d\n",  __FILE__, __LINE__);
//        forever;
//    }
//}

/**
 * external interrupt request 0
 */
ISR( INT0_vect ) {
    printf("\nInterrupt:  INT0_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}

/**
 * external interrupt request 1
 */
ISR( INT1_vect ) {
    printf("\nInterrupt:  INT1_vect%s:%d\n", __FILE__, __LINE__);
    forever;
}


///**
// * external interrupt request 2
// */
//ISR( INT2_vect ) {
//    printf("\nInterrupt:  INT2_vect %s:%d\n", __FILE__, __LINE__);
//    forever;
//}


/**
 * pin change interrupt request 0
 */
ISR( PCINT0_vect ) {
    printf("\nInterrupt:  PCINT0_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}


/**
 * pin change interrupt request 1
 */
ISR( PCINT1_vect ) {
    printf("\nInterrupt:  PCINT1_vect %s:%d\n", __FILE__, __LINE__);
    
    forever;
}

/**
 * pin change interrupt request 2
 */
ISR( PCINT2_vect ) {
    printf("\nInterrupt:  PCINT2_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}

///**
// * pin change interrupt request 3
// */
//ISR( PCINT3_vect ) {
//    printf("\nInterrupt:  PCINT3_vect %s:%d\n",  __FILE__, __LINE__);
//    forever;
//}

/**
 * timer0/counter0: 8-bit counter overflow
 */
ISR( TIMER0_OVF_vect ) {
    printf("\nInterrupt:  TIMER0_OVF_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}

/**
 * timer0/counter0 compare matches A
 */
ISR( TIMER0_COMPA_vect ) {
    printf("\nInterrupt:  TIMER0_COMPA_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}

/**
 * timer0/counter0 compare matches B
 */
ISR( TIMER0_COMPB_vect ) {
    printf("\nInterrupt:  TIMER0_COMPB_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}


/**
 * timer1/counter1: 16-bit counter overflow
 */
ISR( TIMER1_OVF_vect ) {
    printf("\nInterrupt:  TIMER1_OVF_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}

/**
 * timer1/counter1 compare matches A
 */
ISR( TIMER1_COMPA_vect ) {
    printf("\nInterrupt:  TIMER1_COMPA_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}

/**
 * timer1/counter1 matches B
 */
ISR( TIMER1_COMPB_vect ) {
    printf("\nInterrupt:  TIMER1_COMPB_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}

/**
 * timer1/counter1: captured event triggers interrupt => timer1 is saved to ICR1
 */
ISR( TIMER1_CAPT_vect ) {
    printf("\nInterrupt:  TIMER1_CAPT_vect %s:%d\n", __FILE__, __LINE__);
    forever;
}

/**
 * timer2/counter2 compare matches A
 */
ISR( TIMER2_COMPA_vect ) {
    printf("\nInterrupt:  TIMER2_COMPA_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}

/**
 * timer2/counter2 compare matches B
 */
ISR( TIMER2_COMPB_vect ) {
    printf("\nInterrupt:  TIMER2_COMPB_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}

/**
 * timer2/counter2: 8-bit counter overflow
 */
ISR( TIMER2_OVF_vect ) {
    printf("\nInterrupt:  TIMER2_OVF_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}


///**
// * USART0 transmission complete
// */
//ISR( USART0_TX_vect ) {
//    printf("\nInterrupt: USART0_TX_vect %s:%d\n",  __FILE__, __LINE__);
//    forever;
//}

///**
// * USART0 data register is empty
// */
//ISR( USART0_UDRE_vect ) {
//    printf("\nInterrupt: USART0_UDRE_vect %s:%d\n",  __FILE__, __LINE__);
//    forever;
//}
//
///**
// * USART0 receive complete
// */
//ISR( USART0_RX_vect ) {
//	printf("\nInterrupt: USART0_RX_vect %s:%d\n",  __FILE__, __LINE__);
//	forever;
//}


///**
// * serial peripherial interface: serial transfer complete
// */
//ISR( SPI_STC_vect ) {
//    printf("\nInterrupt: SPI_STC_vect %s:%d\n",  __FILE__, __LINE__);
//    forever;
//}
//
///**
// * USART1 receive complete
// */
//ISR( USART1_RX_vect ) {
//    printf("\nInterrupt:  USART1_RX_vect %s:%d\n",  __FILE__, __LINE__);
//    forever;
//}
//
///**
// * USART1 transmission complete
// */
//ISR( USART1_TX_vect ) {
//    printf("\nInterrupt:  USART1_TX_vect %s:%d\n",  __FILE__, __LINE__);
//    forever;
//}
//
///**
// * USART1 data register is empty
// */
//ISR( USART1_UDRE_vect ) {
//    printf("\nInterrupt:  USART1_UDRE_vect %s:%d\n",  __FILE__, __LINE__);
//    forever;
//}

/**
 * Analog to digital converter ready
 */
ISR( ADC_vect ) {
    printf("\nInterrupt:  ADC_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}

/**
 * analog comparator: comparison is ready
 */
ISR( ANALOG_COMP_vect ) {
    printf("\nInterrupt:  ANALOG_COMP_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}

/**
 * EEPROM ready
 */
ISR( EE_READY_vect ) {
    printf("\nInterrupt:  EE_READY_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}

/**
 * Store Program Memory ready
 */
ISR( SPM_READY_vect ) {
    printf("\nInterrupt:  SPM_READY_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}

/**
 * 2-wire serial interface
 */
ISR( TWI_vect ) {
    printf("\nInterrupt:  TWI_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}

/**
 * watch dog timer
 */
ISR( WDT_vect ) {
    printf("\nInterrupt:  WDT_vect %s:%d\n",  __FILE__, __LINE__);
    forever;
}
