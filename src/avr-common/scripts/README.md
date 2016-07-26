<!--- @author: Raoul Rubien 2011 -->

## CLI options (Make):
for a complete list use *make help*
```
make flash           flash device (stk500v2)
make verify          compare flash to hex
make erase           erase flash
make fuse            write fuses (h|l|e fuse)

make rfuses          read most fuses (h|l|e fuse)
make rlfuse          read low fuse
make rhfuse          read hfuse
make refuse          read extra fuse

make listen          monitors debug output from usart1
make avrterminal     connet to device and start avr terminal

make help            display make targets
make info            display help information
```


## Startup with Eclipse generator:
  * mkdir build && cd build
  * cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../src
  * Eclipse -> File -> Import -> Existing project into workspace: Browse to build path -> Finish
  * Add includepath: -> project -> properties -> C/C++ include paths and symbols "/usr/lib/avr/include"

## Requirements - programs/packages:
  * cmake
  * gcc-avr
  * avr-libc
  * binutils-avr
  * srecord (manipulate EPROM load files)
  * cu (for rs232 monitoring)
  * avrdude
  * simulavr
  * ddd
  * gdb
  * xdot

## Udev rules (avrdude may require root rights):
```
 $ echo 'sudo echo 'SUBSYSTEM=="usb", SYSFS{idVendor}=="1781", SYSFS{idProduct}=="0c9f", GROUP="adm", MODE="0666"' > /etc/udev/rules.d/10-avrdude.rules' | sudo sh
 $ sudo restart udev
```


