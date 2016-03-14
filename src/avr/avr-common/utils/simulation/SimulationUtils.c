/**
 * @author Raoul Rubien 2015
 */

#include <avr/io.h>

static void writeToUartRegister(const char *string);

/**
 * {@link see #writeToUartRegister(char *)}
 */
void writeToUart(const char *string) {
    writeToUartRegister(string);
}

/**
 * Write a string to UDR register that is monitored in simulation.
 */
static void writeToUartRegister(const char *string) {

    if (string != 0) {
        while (*string != 0) {
            UDR = *string;
        }
        UDR = '\n';
    }
}