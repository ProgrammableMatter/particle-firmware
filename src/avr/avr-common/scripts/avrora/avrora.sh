#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

jvm="java -jar"
simulator="$DIR/avrora.jar"
args="$@"

echo "starting simulation: $jvm $simulator $args"
$jvm $simulator $args
