#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`
OLDPWD=`pwd`

cd $SCRIPTPATH

MODULES=("avr-cycles" "avrora-particle-platform" "cstruct-to-json" "elf-address-to-label" "network-visualization")

# init and update modules
git submodule init
git submodule update

# define branch check out modules
cd ./modules
for moduleName in "${MODULES[@]}"
do
  cd $moduleName
  git checkout master
  git pull
  cd ..
done

cd $SCRIPTPATH
# run modules' buld.sh
for moduleName in "${MODULES[@]}"
do
  if [ -f modules/$moduleName/build.sh ] ; then
    ./modules/$moduleName/build.sh
  fi
done

cd $OLDPWD
