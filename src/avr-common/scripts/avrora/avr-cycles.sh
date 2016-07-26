#!/bin/bash

#
# @author Raoul Rubien 2016
#

CMCU=$1
HEX_FILE=$2

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

/usr/bin/avr-objdump -zS $HEX_FILE | /usr/bin/python $DIR/avr-cycles.py $CMCU

