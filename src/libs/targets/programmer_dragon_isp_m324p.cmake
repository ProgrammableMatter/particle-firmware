# @author: Raoul Rubien 2011

SET(PROGRAMMER dragon_isp)
#SET(PROGRAMMER dragon_pp)
#SET(PROGRAMMER dragon_hvsp)
#SET(PROGRAMMER dragon_jtag)
#SET(PROGRAMMER dragon_dw)

SET(PROGRAMMER_PORT usb)
#SET(PROGRAMMER_BAUD 1200)
#SET(PROGRAMMER_BAUD 2400)
#SET(PROGRAMMER_BAUD 4800)
#SET(PROGRAMMER_BAUD 9600)
#SET(PROGRAMMER_BAUD 19200)
#SET(PROGRAMMER_BAUD 38400)
#SET(PROGRAMMER_BAUD 57600)
#SET(PROGRAMMER_BAUD 152000)
#SET(PROGRAMMER_BAUD 230400)
SET(PROGRAMMER_BAUD 460800)

SET(PROGRAMMER_TARGET m324p)

# -y count erase cycles
# -v verbose
# -F override invalid signature check
# -u disable save mode
# -V do not verify
# -D disable auto erase for flash memory
# -t enter terminal mode (debug only)
SET(PROGRAMMER_EXTRA_FLAGS -yuvF)

