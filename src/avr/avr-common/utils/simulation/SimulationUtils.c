/**
 * @author Raoul Rubien 2016
 */

#include <avr/io.h>

/**
 * Write a string to UDR register that is monitored in simulation.
 */
static void __writeToUartRegister(const char *string) {
    if (string != 0) {
        while (*string != 0) {
            UDR = *string;
        }
        UDR = '\n';
    }
}

/**
 * {@link see #writeToUartRegister(char *)}
 */
void writeToUart(const char *string) {
    __writeToUartRegister(string);
}