# @author: Raoul Rubien 2015

SET(PROGRAMMER ft232rl_dasa)

#SET(PROGRAMMER_PORT usb)
SET(PROGRAMMER_PORT /dev/ttyUSB0)

SET(PROGRAMMER_BAUD 1200)
#SET(PROGRAMMER_BAUD 2400)
#SET(PROGRAMMER_BAUD 4800)
#SET(PROGRAMMER_BAUD 9600)
#SET(PROGRAMMER_BAUD 19200)
#SET(PROGRAMMER_BAUD 38400)
#SET(PROGRAMMER_BAUD 57600)
#SET(PROGRAMMER_BAUD 152000)
#SET(PROGRAMMER_BAUD 230400)
#SET(PROGRAMMER BAUD 460800)

SET(PROGRAMMER_TARGET t20)

# -y count erase cycles - not supported
# -v verbose
# -F override invalid signature check
# -u disable save mode
# -V do not verify
# -D disable auto erase for flash memory
# -t enter terminal mode (debug only)
SET(PROGRAMMER_EXTRA_FLAGS "-uvveC${CMAKE_SOURCE_DIR}/avrdude.conf")
