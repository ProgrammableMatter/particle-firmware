#!/bin/bash

#
# @author: Raoul Rubien 2011
#

# waits for following cli args: [programmer] [target] [baud] [port] [flags] [input]

SCRIPT_PATH="$(readlink -f $(dirname "$0"))"
SCRIPT_REL_PATH="$(dirname "$0")"
SCRIPT_NAME="${0##*/}"
ARG_COUNT=${#@}
source $SCRIPT_PATH/utils.sh

programmer="$1"
target="$2"
baud="$3"
port="$4"
flags="$5"
src="$6"

echo ""
echo "[$0]"
echo "programmer=$programmer"
echo "target=$target"
echo "baud=$baud"
echo "port=$port"
echo "flags=$flags"
echo "src=$src"
echo ""

checkInstalled "avrdude"
assertSuccess $? "ERROR: Missing installed programs."

command="avrdude -v -c $programmer -p $target -b $baud -P $port -U flash:v:$src:a $flags"
echo "$command"
`$command`
assertSuccess $? "Verifying FAILED!"
