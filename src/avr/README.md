## elf-mapper
maps addresses found in *readelf -a* to labels

## particle
implementation of a programmable-matter particle

## particle-protoboard-hw-test
simple program for testing all outputs of a board; usually used once after assembling the pcb

## paricle-unit-tests
unittests

## avr-common contains
    * mcu independent implementations
    * bash scripts (i.e. erase, flash, read/write fuses)
    * cmake settings (i.e. programmer, mcu)
##  other folders should contain projects that, in case of atmega projects:
* make use of symlinked folders i.e.:
	* ln -s ../avr-common/targets targets
	* ln -s ../avr-common/scripts scripts 
	* ln -s ../avr-common/utils/common common
	
	
## build from source
* mkdir build
* cd build
* cmake ../src/<yourproject>
* make && make flash	

* for Eclipse
* cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../src/<yourproject>
* eclipse -> new project -> C/C++ -> Makefile Project with Existing Code -> select AVR-GCC Toolchain
    * project name should be different from: executable, build folder
* add includepath: -> project -> properties -> C/C++ include paths and symbols "/usr/lib/avr/include"

* all targets can be listed using
 * *make help* i.e.
 ```
... all (the default if no target is provided)
... clean
... depend
... edit_cache
... rebuild_cache
... avrterminal
... erase
... flash
... fuse
... info
... listen
... refuse
... rfuses
... rhfuse
... rlfuse
... verify
... avrora-analyze-stack
... avrora-cfg
... avrora-elf-dump
... avrora-inter-procedural-side-effect-analysis
... avrora-simulate
... avrora-test
... test
... <a_binary>.elf
...
```
