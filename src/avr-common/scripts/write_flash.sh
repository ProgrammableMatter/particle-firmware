#!/bin/bash
# @author: Raoul Rubien 2011

# waits for following cli args: [programer] [target] [baud] [port] [flags] [input]

SCRIPT_PATH="$(readlink -f $(dirname $0))"
SCRIPT_REL_PATH="$(dirname $0)"
SCRIPT_NAME="${0##*/}"
ARG_COUNT=${#@}
source $SCRIPT_PATH/utils.sh

programmer="$1"
target="$2"
baud="$3"
port="$4"
flags="$5"
input="$6"
flashExt=".flash"
eepromExt=".eeprom"
flash="$input$flashExt"
eeprom="$input$eepromExt"

rm -f $flash $eeprom > /dev/null 2>&1
checkInstalled "avr-objcopy avrdude srec_cat"
assertSuccess $? "ERROR: Missing installed programs."

echo ""
echo "[$0]"
echo "programmer=$programmer"
echo "target=$target"
echo "baud=$baud"
echo "port=$port"
echo "flags=$flags"
echo "input=$input"
echo "flash=$flash"
echo "eeprom=$eeprom"
echo ""

#remove debug sections
avr-objcopy  -R .stab -R .stabstr -R .comment $input $input 

# extract flash
avr-objcopy  -j .text -j .data -O ihex $input $flash
#avr-objcopy  -j .text -j .data  -j .bss -O ihex   $input $flash
assertSuccess $? "FAILED to copy .text from [$input] -> [$flash]"
avrdude -c $programmer -p $target -b $baud -P $port -U flash:w:$flash:i $flags
assertSuccess $? "FAILED to write binary to device."


# extract eeprom
#avr-objcopy  -j .eeprom   --no-change-warnings --change-section-lma .eeprom=0 -O ihex $input $eeprom
#assertSuccess $? "FAILED to copy .eeprom from [$input] -> [$eeprom]"
# write to target
#avrdude -c $programmer -p $target -b $baud -P $port -U flash:w:$flash:a -U eeprom:w:$eeprom:a $flags


