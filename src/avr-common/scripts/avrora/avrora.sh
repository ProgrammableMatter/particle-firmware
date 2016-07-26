#!/bin/bash

#
# @author Raoul Rubien 2015
#

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

jvm="java -jar"

simulator="$DIR/avrora.jar"
simulatorInLocalMavenRepo="$HOME/.m2/repository/at/tugraz/ist/avrora/particleplatform/0.0.1-SNAPSHOT/particleplatform-0.0.1-SNAPSHOT-jar-with-dependencies.jar"

args="$@"

if [ -f "$simulatorInLocalMavenRepo" ] ; then
  simulator="$simulatorInLocalMavenRepo"
fi

echo "starting simulation: $jvm $simulator $args"
$jvm $simulator $args
