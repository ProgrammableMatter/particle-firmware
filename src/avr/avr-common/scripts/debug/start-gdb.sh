#!/bin/bash

#
# @author Raoul Rubien 2015
#

SCRIPT_PATH="$(readlink -f $(dirname "$0"))"
SCRIPT_NAME="${0##*/}"

function main ()
{

    if [ -z "$1" -o -z "$2" ]
    then
      usage
    else
        echo "starting debugger for device [$1] with hex [$2]"
        
        # device is "-mmcu=atmega8" but need without "-mmcu="
        prefix="-mmcu="
        device=${1#$prefix}
        simulavr; simulavr --gdbserver --device $device &
        simulavrpid=$!
        ddd --debugger avr-gdb --eval-command 'target remote localhost:1212' $2
        kill $simulavrpid
    fi
}

function usage {
  echo ""
  echo "$SCRIPT_NAME [device] [hexfile.elf]"
  echo ""
  echo "for a list of defices use 'similavr --device"
  echo ""
}

main $@

