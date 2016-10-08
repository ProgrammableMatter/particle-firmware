/**
 * @author Raoul Rubien 08.10.2016
 *
 * USART related arguments.
 */

#pragma once

#ifdef __AVR_ATtiny1634__
/**
 * Baud rate for USART1 output (RS232).
 * Default setup is 8bit, no parity bit, 1 stop bit or better known as "8N1".
 * Note: The USART transmission is implemented for AVR ATtiny1634 in DEBUG compilation only.
 */
#  define STDOUT_UART_BAUD_RATE (19200)

#endif