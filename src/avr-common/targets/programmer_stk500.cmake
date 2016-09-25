# @author Raoul Rubien 2016

SET(PROGRAMMER stk500)
#SET(PROGRAMMER stk500hvsp)
#SET(PROGRAMMER stk500pp)
#SET(PROGRAMMER stk500v1)
#SET(PROGRAMMER stk500v2)

SET(PROGRAMMER_PORT /dev/ttyUSB0)
SET(PROGRAMMER_PORT2 /dev/ttyUSB1)
SET(PROGRAMMER_PORT3 /dev/ttyUSB2)

SET(PROGRAMMER_BAUD 115200)

# -y count erase cycles
# -v verbose
# -F override invalid signature check
# -u disable save mode
# -V do not verify
# -D disable auto erase for flash memory
# -t enter terminal mode (debug only)
# -E exitspec: reset, noreset, vcc, novcc, d_high, d_low; not supported by all programmer i.e. ft232rl_dasa
SET(PROGRAMMER_EXTRA_FLAGS -VuvFEnoreset)
