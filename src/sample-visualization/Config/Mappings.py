wireToFloatValueMapping = {"high": 1.0, "low": 0.0,
                           "hi": 1.0, "lo": 0.0,
                           }

interruptToFloatValueMapping = {"posted": 1.0, "unposted": 0.0,
                                "enabled": 1.0, "disabled": 0.0,
                                "invoke": 1.0, "return": 0.0,
                                }

interruptToNumberMapping = {"TX_RX_TIMER_TOP": "#7",
                            "TX_RX_TIMER_CENTER": "#8",
                            "TX_RX_TIMEOUT_INTERRUPT": "#20",
                            "NORTH_RECEPTION": "#19",
                            "EAST_RECEPTION": "#3",
                            "SOUTH_RECEPTION": "#2",
                            }

interruptToNameMapping = {"TX_RX_TIMER_TOP": "counter1 comp. A (top)",
                          "TX_RX_TIMER_CENTER": "counter1 comp. B (center)",
                          "TX_RX_TIMEOUT_INTERRUPT": "counter0 comp. A",
                          "NORTH_RECEPTION": "north reception",
                          }

interruptDomainToNameMapping = {"post": "un-/posting",
                                "enable": "en-/disabling",
                                "invoke": "call/return",
                                }

charOutToHumanReadableAnnotation = {
    "'I'": "interpret",
    "'i'": "interpret out",
    "'c'": "clr buff",
    "'E'": "enum east",
    "'e'": "enum south",
    "'P'": "main loop in",
    "'p'": "main loop out",

    "'D'": "INITIATOR_STATE_TYPE_IDLE",
    "'T'": "INITIATOR_STATE_TYPE_TRANSMIT",
    "'F'": "INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED",
    "'R'": "INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE",
    "'A'": "INITIATOR_STATE_TYPE_TRANSMIT_ACK",
    "'f'": "INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED",
    "'d'": "RECEPTIONIST_STATE_TYPE_IDLE",
    "'r'": "RECEPTIONIST_STATE_TYPE_RECEIVE",
    "'a'": "RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK",
    "'g'": "RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED",
    "'h'": "RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE",

    "'M'": "comm: transaction finished",
    "'V'": "comm: transaction restart",
    "'W'": "state: wait for enum.",

    "'0'": "0",
    "'1'": "1",
    "'|'": "|",
    "'+'": "+",
    "'x'": "x",
    "'X'": "X",

    "'b'" : "counter timeout",
    "'u'" : "decode/interpreter error",
}