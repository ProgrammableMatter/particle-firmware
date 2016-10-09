/**
 * @author Raoul Rubien  07.10.2016
 */

#pragma once

#ifdef NDEBUG
#  define setupUart(...)
#else
#  if defined(__AVR_ATtiny1634__)
#    include <avr/io.h>
#    include <stdio.h>
#    include "common/PortInteraction.h"
#    include "uc-core/configuration/Stdout.h"
#    define __STDOUT_BAUD_PRESCALE (((F_CPU / (STDOUT_UART_BAUD_RATE * 16UL))) - 1)

uint8_t uartPutchar(char byte) {
//    if (byte == '\n') {
//        uartPutchar('\r');
//    }
    while ((UCSR1A getBit bit(UDRE1)) == 0);
    UDR1 = byte;
    return 0;
}

FILE __UART_STD_OUT = FDEV_SETUP_STREAM(uartPutchar, NULL, _FDEV_SETUP_WRITE);

void setupUart(void) {
    // set baud rate
    UBRR1H = (uint8_t) (__STDOUT_BAUD_PRESCALE >> 8);
    UBRR1L = (uint8_t) __STDOUT_BAUD_PRESCALE;

    // no 2x speed, no multiprocessor
    UCSR1A = 0;

    // enable receiver, disable interrupts
    UCSR1B = 0;
    UCSR1B setBit (
        // bit(RXCIE1)
        // bit(TXCIE1)
        // | bit(UDRIE1)
        // | bit(RXEN1)
            bit(TXEN1)
        // | bit(UCSZ12)
        // | bit(RXB81)
        // | bit(TXB81)
    );

    // frame format 8data, 1stop bit
    UCSR1C = 0;
    UCSR1C setBit (
        // bit(UMSEL11)
        // | bit (UMSEL10)
        // | bit(UPM11)
        // | bit(UPM10)
        // | bit(USBS1)
            bit(UCSZ11)
            | bit(UCSZ10)
        // | bit(UCPOL1)
    );

    stdout = &__UART_STD_OUT;
}
#  else
#    define setupUart(...)
#  endif
#endif