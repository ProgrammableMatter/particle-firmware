* libs folder contains
    * mcu independent implementations
    * bash scripts (i.e. erase, flash, read/write fuses)
    * cmake settings (i.e. programmer, mcu)

* any other folder equals to prjects
    * that use cmake settings (libs/targets) and bash scripts (libs/scripts)
    * and may use parts of the libraries (libs/utils)

* build from source
    * mkdir build
    * cd build
    * cmake ../src/<yourproject>
    * make && make flash	

    * for Eclipse
	* cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../src/<yourproject>
	* eclipse -> new project -> from existing source -> select build folder
	* add includepath: -> project -> properties -> C/C++ include paths and symbols "/usr/lib/avr/include"
