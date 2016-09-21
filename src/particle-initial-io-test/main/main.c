/**
 * @author Raoul Rubien 2015
 */

#include <util/delay.h>
#include <common/common.h>
#include <common/PortADefinition.h>
#include <common/PortBDefinition.h>
#include <common/PortCDefinition.h>

FUSES = {
        .low = LFUSE_DEFAULT,
        .high = HFUSE_DEFAULT,
        .extended = EFUSE_DEFAULT,
};

void __init(void) { }

void toggleOutputPins(void) {
    ADir setIn Pin4; // rx pin
    CDir setIn Pin5; // rx pin

    ADir setOut (Pin0 | Pin1 | Pin2 | Pin3 | Pin5 | Pin6 | Pin7);
    BDir setOut (Pin0 | Pin1 | Pin2 | Pin3);
    CDir setOut (Pin0 | Pin1 | Pin2 | Pin3 | Pin4);

    forever {
        AOut toggleBit (Pin0 | Pin1 | Pin2 | Pin3 | Pin5 | Pin6 | Pin7);
        BOut toggleBit (Pin0 | Pin1 | Pin2 | Pin3);
        COut toggleBit (Pin0 | Pin1 | Pin2 | Pin3 | Pin4);
        _delay_ms(1.0);
    }
}

void toggleReceptionPins(void) {
    ADir setIn(Pin0 | Pin1 | Pin2 | Pin3 | Pin4 | Pin5 | Pin6 | Pin7);
    BDir setIn(Pin0 | Pin1 | Pin2 | Pin3);
    CDir setIn(Pin0 | Pin1 | Pin2 | Pin3 | Pin4 | Pin5);

    BDir setOut Pin2;

    ADir setOut Pin2;
    CDir setOut Pin4;

    AOut setHi Pin2;
    COut setHi Pin4;

    ADir setOut Pin4; // rx pin
    CDir setOut Pin5; // rx pin

    forever {
        BOut toggleBit Pin2; // for trigger
        AOut toggleBit Pin4;
        COut toggleBit Pin5;
        _delay_ms(1.0);
    }

}

void toggleMosFets(void) {
    ADir setIn(Pin0 | Pin1 | Pin2 | Pin3 | Pin4 | Pin5 | Pin6 | Pin7);
    BDir setIn(Pin0 | Pin1 | Pin2 | Pin3);
    CDir setIn(Pin0 | Pin1 | Pin2 | Pin3 | Pin4 | Pin5);

    BDir setOut Pin2;

    ADir setOut (Pin2 | Pin3);
    CDir setOut (Pin0 | Pin4);

    forever {
        BOut toggleBit Pin2; // for trigger
        AOut toggleBit (Pin2 | Pin3);
        COut toggleBit (Pin0 | Pin4);
        _delay_ms(1.0);
    }
}


void togglePinsSubsequently(void) {
    uint8_t bitMask;
    ADir setIn(Pin0 | Pin1 | Pin2 | Pin3 | Pin4 | Pin5 | Pin6 | Pin7);
    BDir setIn(Pin0 | Pin1 | Pin2 | Pin3);
    CDir setIn(Pin0 | Pin1 | Pin2 | Pin3 | Pin4 | Pin5);

    forever {
        bitMask = Pin0;
        while (bitMask > 0) {
            ADir setOut bitMask;
            AOut toggleBit bitMask;
            _delay_us(500);
            AOut toggleBit bitMask;
            _delay_us(500);
            ADir setIn bitMask;
            bitMask <<= 1;
        }

        bitMask = Pin0;
        while (bitMask > 0) {
            BDir setOut bitMask;
            BOut toggleBit bitMask;
            _delay_us(500);
            BOut toggleBit bitMask;
            _delay_us(500);
            BDir setIn bitMask;
            bitMask <<= 1;
        }

        bitMask = Pin0;
        while (bitMask > 0) {
            CDir setOut bitMask;
            COut toggleBit bitMask;
            _delay_us(500);
            COut toggleBit bitMask;
            _delay_us(500);
            CDir setIn bitMask;
            bitMask <<= 1;
        }
    }
}

int main(void) {
//    toggleOutputPins();
//    toggleReceptionPins();
    toggleMosFets();
//    togglePinsSubsequently();
}



