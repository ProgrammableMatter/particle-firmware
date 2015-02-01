#include <avr/interrupt.h>

#include <common/IntTypes.h>
#include "fuses.h"

extern "C" {
#include "lcd/LcdHD4478.h"
};

/**
 * free function declaration
 */
void init (void);

int main (void)
{
    lcd_init(42);
    forever{}
}

/**
 * initialization before main is called
 */
void init (void)
{
}

