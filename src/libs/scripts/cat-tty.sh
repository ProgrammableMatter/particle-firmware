#!/bin/bash

device="$1"         # serial device
baud_rate="38400"   # bit/sec
parity="-parenb"    # no parity bit
stop_bits="-cstopb" # one stop bit
byte_bits="cs8"			# num byte bits

echo -en "listening to $1 by using settings: $baud_rate $parity $stop_bits $byte_bits \n\n"
stty -F $1 $baud_rate $parity $byte_bits   
cat $1