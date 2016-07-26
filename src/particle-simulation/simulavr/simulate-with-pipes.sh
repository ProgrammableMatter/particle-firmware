#!/bin/bash

#
# @author Raoul Rubien 2015
#

SCRIPT_NAME="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
SCRIPT=`realpath $0`
SCRIPTPATH=`dirname $SCRIPT`

SIMAVR_BIN=$SCRIPTPATH/../../../bin/simulavr/usr/bin/simulavr
CMCU=$1
CPUFREQ=$2
HEXFILE=$3
NUM_ARGS=$#

IN_PIPE=$4
OUT_PIPE=$5

MINARGS=0
MAXARGS=5

function createFifo() {
    if [ ! -f $1 ] ; then
        echo "creating fifo $1"
        mkfifo $1
    else
        echo "skipped creating fifo $1"
    fi
}

function usage() {
    echo
    echo "$SCRIPT_NAME <MCU> <CPUFREQ> <HEXFILE> <IN-PIPE> <OUT-PIPE>"
    echo
    echo
}

function main() {
    if [ "$NUM_ARGS" -eq "$MAXARGS" ] ; then
        createFifo $IN_PIPE
        createFifo $OUT_PIPE

        cmd="$SIMAVR_BIN --device $CMCU --cpufrequency  $CPUFREQ -f $HEXFILE -W 0x20,$OUT_PIPE -R 0x22,$IN_PIPE"
        $cmd
        echo "..."
        echo "run command $cmd"
    else
        usage
    fi
}

main $@
