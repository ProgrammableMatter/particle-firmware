wireToFloatValueMapping = {"high": 1.0, "low": 0.0,
                       "hi": 1.0, "lo": 0.0,}

interruptToFloatValueMapping = {"posted": 1.0, "unposted": 0.0,
                            "enabled": 1.0, "disabled": 0.0,
                            "invoke": 1.0, "return": 0.0,}

interruptToNumberMapping = {"TX_RX_TIMER_TOP": "#7",
                     "TX_RX_TIMER_CENTER": "#8",
                     "TX_RX_TIMEOUT_INTERRUPT": "#20",
                     "NORTH_RECEPTION": "#19"}

interruptToNameMapping = {"TX_RX_TIMER_TOP": "counter1 comp. A (top)",
                   "TX_RX_TIMER_CENTER": "counter1 comp. B (center)",
                   "TX_RX_TIMEOUT_INTERRUPT": "counter0 comp. A",
                   "NORTH_RECEPTION": "north reception"}

interruptDomainToNameMapping = {"post": "un-/posting",
                         "enable": "en-/disabling",
                         "invoke": "call/return"}