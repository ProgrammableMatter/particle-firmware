avr-common
----------
This folder contains:
    * mcu independent implementations
    * bash scripts (i.e. erase, flash, read/write fuses)
    * cmake settings (i.e. programmer, mcu)
This content is mainly meant to be symlinked from other binary generating folders.

other folders should contain:
-----------------------------
* make use of symlinked folders i.e.:
	* ln -s ../avr-common/targets targets
	* ln -s ../avr-common/scripts scripts 
	* ln -s ../avr-common/utils/common common
	
how to build from source
------------------------
* mkdir build
* cd build
* cmake ../src/<yourprojectfolder>
* make && make flash	

* for Eclipse
* cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../src/<yourproject>
* eclipse -> new project -> C/C++ -> Makefile Project with Existing Code -> select AVR-GCC Toolchain
    * project name should be different from: executable, build folder
* add includepath: -> project -> properties -> C/C++ include paths and symbols "/usr/lib/avr/include"

* all targets can be listed using
 * *make help* i.e.

binaries
--------

* particle

Productive firmware.

* particle-simulation

Same as productive firmware but optimized for simulation, logging and debugging.

* particle-initial-io-test

Basic test firmware for physical MCU that drives the MOS-FETs.

* particle-simulation-io-test

Basic test firmware for simulation tests which prove the correct output pins mapping.

* manchester-code-tx-simulation

Manchester signal generator for decoding tests.

* particle-reception-simulation

Firmware for Manchester decoding tests.

* particle-transmission-simulation

Firmware for Manchester coding and decoding tests.

* particle-simulation-heatwires-test

Network command test firmware: heat wires - command.

* particle-simulation-heatwiresrange-test

Network command test firmware: heat wires range - command.

* particle-simulation-setnewnetworkgeometry-test

Network command test firmware: set new network geometry - command.

testing
-------

More about JUnit tests and simulation can be found [here: https://github.com/ProgrammableMatter/avrora-particle-platform/...](https://github.com/ProgrammableMatter/avrora-particle-platform/tree/master/particleplatform).

