#!/bin/bash
# @author: Raoul Rubien 2011

## waits for following cli args: [programer] [target] [baud] [port] [flags] [input]

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
input="$6"
fuseFile="fuses.hex"

lfuse="lfuse.hex"
hfuse="hfuse.hex"
efuse="efuse.hex"

rm -f $hex         > /dev/null 2>&1
rm -f $fuseFile    > /dev/null 2>&1
rm -f $lfuse       > /dev/null 2>&1
rm -f $hfuse       > /dev/null 2>&1
rm -f $efuse       > /dev/null 2>&1

checkInstalled "avr-objcopy avrdude srec_cat"
assertSuccess $? "ERROR: Missing installed programs."

echo ""
echo "[$0]"
echo "programmer=$programmer"
echo "target="$target"
echo "baud=$baud"
echo "port=$port"
echo "flags=$flags"
echo "input="$input"
echo "fuseFile=$fuseFile"
echo ""

echo "extract fuses from [$input] -> [$fuseFile]"
avr-objcopy -j .fuse -O ihex $input $fuseFile --change-section-lma .fuse=0
assertSuccess $? "FAILED extracting .fuse section from [$input] to [$fuseFile]"

echo "extract L-fuse [$lfuse]"
srec_cat $fuseFile -Intel -crop 0x00 0x01 -offset  0x00 -O $lfuse -Intel
assertSuccess $? "ERROR: lfuse mismatch"
cat $lfuse
echo ""

echo "extract H-fuse [$hfuse]"  
srec_cat $fuseFile -Intel -crop 0x01 0x02 -offset -0x01 -O $hfuse -Intel
assertSuccess $? "ERROR: hfuse mismatch"
cat $hfuse
echo ""

echo "extract E-fuse [$efuse]"
srec_cat $fuseFile -Intel -crop 0x02 0x03 -offset -0x02 -O $efuse -Intel
assertSuccess $? "ERROR: efuse mismatch"
cat $efuse
echo ""

# write fuses to device

cmd="avrdude -c $programmer -p $target -b $baud -P $port $flags -U hfuse:w:$hfuse:i -U lfuse:w:$lfuse:i 
#-U efuse:w:$efuse:i"
echo "$cmd"
`$cmd`
assertSuccess $? "FAILED to write H-fuse to target."

