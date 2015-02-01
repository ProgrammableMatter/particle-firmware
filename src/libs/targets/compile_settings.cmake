SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER   avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(CDEBUG    "-gstabs")
SET(CWARN     "-Wall -Wstrict-prototypes")
SET(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT      "-Os")

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS   "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

SET(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU}           ${CDEFS} ${CINCS} ${COPT}")

SET(CMAKE_C_FLAGS   ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

