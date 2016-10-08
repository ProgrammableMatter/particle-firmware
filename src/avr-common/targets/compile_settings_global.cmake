# @author Raoul Rubien 2016

SET(CMAKE_VERBOSE_MAKEFILE True)
SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(CXXSTANDARD "-std=gnu99")

SET(CWARN "-Wall -Werror -Wvolatile-register-var -Wunused-variable -Wunused-function -Wunused-label -Wunused-value -Wextra -Wshadow -Wstrict-prototypes -Waddress -Wredundant-decls")
SET(CXXWARN "-Wall -Werror -Wvolatile-register-var -Wunused-variable -Wunused-function -Wunused-label -Wunused-value -Wextra -Wshadow -Wstrict-prototypes -Waddress -Wredundant-decls")

# Do not use "-Wl,--gc-sections"!
SET(CTUNING " -fpack-struct -funsigned-bitfields -funsigned-char -fdata-sections -fdce -ffunction-sections ${CTUNING}")

SET(PRINTF_LIB_FLOAT "${PRINTF_LIB_FLOAT}")

SET(COPT "${COPT}")
SET(CDEBUG "${CDEBUG}")

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${PRINTF_LIB_FLOAT} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING} ${DEFINED_MACROS}")
SET(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${PRINTF_LIB_FLOAT}${COPT} ${CXXWARN} ${CXXSTANDARD} ${CTUNING} ${DEFINED_MACROS}")

SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})
