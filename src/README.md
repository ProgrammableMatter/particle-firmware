Particle firmware project
=========================
This project implements a particle that, if connected to a
particle network, acts as a programmable entity. The network as a whole acts
as programmable matter (see <http://arxiv.org/abs/1402.2507>).
* Videos of several test cases can be watched [here](https://drive.google.com/drive/folders/0Bx3U4CEhKK8dWDU4OHdjRE1Dblk?usp=sharing).
* Details about synchronization methods and tests' setup: [Syncrhonization Listing](https://github.com/ProgrammableMatter/particle-firmware/tree/master/src/avr-common/utils/uc-core/synchronization)

Folder structure
----------------
### avr-common 
This content is mainly meant to be symlinked from other sub project folders.
The folder contains:

* utils: mcu independent implementations
* scripts: bash scripts (i.e. avr erase, avr flash, avr read and write fuses and much more)
* targets: cmake settings (i.e. programmer, mcu, debugger)

### Other folder
Other sub projects may use symlinked folders i.e.:

* ln -s ../avr-common/targets targets
* ln -s ../avr-common/scripts scripts 
* ln -s ../avr-common/utils/common common

### Build from source
#### Console

    mkdir build
    cd build
    cmake ../src/<yourprojectfolder>
    make && make flash

#### Intellij - CLion
Import project from source.

#### Eclipse
Make use of the cmake Eclipse project generator:

    cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../src/<yourproject>
    
1. eclipse -> new project -> C/C++ -> Makefile Project with Existing Code -> select AVR-GCC Toolchain
1. project name should be different from: executable, build folder
1. add includepath: -> project -> properties -> C/C++ include paths and symbols "/usr/lib/avr/include"
1. all targets can be listed using: **make help\*** i.e. see [binaries](#binaries).
    
Binaries
--------

| Folder | Purpose | 
|:-------|:--------|
| particle | Productive firmware.
| particle-simulation| Same as productive firmware but optimized for simulation, logging and debugging.
| particle-initial-io-test |  Basic test firmware for physical MCU that drives the MOS-FETs.
| particle-simulation-io-test | Basic test firmware for simulation tests which prove the correct output pins mapping.
| manchester-code-tx-simulation | Manchester signal generator for decoding tests.
| particle-reception-simulation | Firmware for Manchester decoding tests.
| particle-transmission-simulation | Firmware for Manchester coding and decoding tests.
| particle-simulation-heatwires-test | Network command test firmware: heat wires - command.
| particle-simulation-heatwiresrange-test | Network command test firmware: heat wires range - command.
| particle-simulation-setnewnetworkgeometry-test | Network command test firmware: set new network geometry - command.

Testing the firmware
--------------------

More about JUnit tests and the platform simulation can be found [here](https://github.com/ProgrammableMatter/avrora-particle-platform).

