# @author: Raoul Rubien 2016

SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(CXXSTANDARD "")
SET(CWARN "-Wall -Wstrict-prototypes -Werror")
SET(CXXWARN "-Wall -Werror")
#SET(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fwhole-program")
SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
#SET(COPT "-Os")
SET(COPT "-O3")

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN}   ${CSTANDARD}   ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CXXWARN} ${CXXSTANDARD} ${CEXTRA}")

SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

