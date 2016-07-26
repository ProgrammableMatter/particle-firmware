#!/bin/bash

#
# @author: Raoul Rubien 2011
#

# waits for following cli args: [programmer] [target] [baud] [port] [flags]

SCRIPT_PATH="$(readlink -f $(dirname "$0"))"
SCRIPT_REL_PATH="$(dirname "$0")"
SCRIPT_NAME="${0##*/}"
ARG_COUNT=${#@}
source $SCRIPT_PATH/utils.sh

programmer="$1"
target="$2"
baud="$3"
port="$4"
flags="$5 -e"

checkInstalled "avrdude"

avrdude -c $programmer -p $target -b $baud -P $port $flags
assertSuccess $? "FAILED to erase flash."
