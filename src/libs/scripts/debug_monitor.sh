#!/bin/bash
# @author: Raoul Rubien 2011

SCRIPT_PATH="$(readlink -f $(dirname "$0"))"
SCRIPT_REL_PATH="$(dirname "$0")"
SCRIPT_NAME="${0##*/}"
ARG_COUNT=${#@}
source $SCRIPT_PATH/utils.sh

device="$1"         # serial device
baud_rate="$2"      # bit/sec
parity="-parenb"    # no parity bit
stop_bits="-cstopb" # one stop bit
byte_bits="cs8"     # num byte bits

checkInstalled "cu"
assertSuccess $? "ERROR: Missing installed programs."


#echo -en "listening to $1 by using settings: stty -F $1 $baud_rate $parity $stop_bits $byte_bits \n\n"
#stty -F $1 $baud_rate $parity $byte_bits  
#cat $1 

echo ""
echo -en "listening to $1 by using settings [line=$1] [baud=$baud_rate]\n\n"
echo "<ENTER> to start"
echo -en "~. to exit session\n"
read x

if [ "$x" == "~." ] ; then 
    exit 0
fi

cu -l $1 -s $baud_rate --parity=none --nostop
 
