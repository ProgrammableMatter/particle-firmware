# @author Raoul Rubien 2016

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
# -V do not verify, specially for attiny20 because if (flash_size % 4 != 0) verification will fail. See: http://lists.nongnu.org/archive/html/avrdude-dev/2015-03/msg00000.html.
# -D disable auto erase for flash memory
# -t enter terminal mode (debug only)
# -E exitspec: reset, noreset, vcc, novcc, d_high, d_low; not supported by all programmer i.e. ft232rl_dasa
SET(PROGRAMMER_EXTRA_FLAGS "-VuveC${PROJECTS_SOURCE_ROOT}/avr-common/targets/avrdude_ft232rl-dasa_attiny20.conf")

# hack to release RTS and let device leave reset mode
SET(POST_PROGRAMMING_COMMAND "&&" "python" "-c'import" "serial" "\\\;" "serial.Serial(\"${PROGRAMMER_PORT}\").setRTS(0)'")
