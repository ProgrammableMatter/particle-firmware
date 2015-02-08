// @author: Raoul Rubien 2011

#ifndef __fuses_h__
#define __fuses_h__

#include <avr/io.h>

/**
 * this obscure section defines the fuse bits that are later written in the elf file
 * the bits can be found in the .fuse section and extracted by using "avr-objcopy -j .fuse"
 * for more inforamtion see /usr/lib/avr/include/avr/fuse.h
 * also see page 295 in ATmega164/324p/664p documentation
 *
 *//*

(0) ... programmed   => on
(1) ... unprogrammed => off

Clock sources:
1111 - 1000 low power xtal osc
0111 - 0110 full swing xtal osc
0101 - 0100 low f xtal osc
0011        internal 128 kHz
0010        internal rc osc
0000        external clock
0001        reserved
*/

FUSES =
{
//      .low = LFUSE_DEFAULT,
//      .high = (FUSE_BOOTSZ0 & FUSE_BOOTSZ1 & FUSE_EESAVE & FUSE_SPIEN & FUSE_JTAGEN),
//      .extended = EFUSE_DEFAULT,

		/*.low      = */(0b11100000),/*
		Low Fuse Byte
		b0 FUSE_CKSEL0      clock source           (0)
		b1 FUSE_CKSEL1      clock source           (1)
		b2 FUSE_CKSEL2      clock source           (0)
		b3 FUSE_CKSEL3      clock source           (0)
		b4 FUSE_SUT0        select startup time    (0)
		b5 FUSE_SUT1        select startup time    (1)
		b6 FUSE_CKOUT       clock output on PORTB1 (1)
		b7 FUSE_CKDIV8      divide clock by 8      (0)*/

		//*.high     = */ (0b11011111),/*
		/*.high     = */ (0b11011001),/*
		High Fuse Byte
		b0 FUSE_BODLEVEL0   brown out detector trigger level (1)
		b1 FUSE_BODLEVEL1   brown out detector trigger level (1)
		b2 FUSE_BODLEVEL2   brown out detector trigger level (1)
		b3 FUSE_EESAVE      EEPROM preserved through chip erase (1) // i.e. while firmware update
		b4 FUSE_WDTON       watchdog timer      	(1)
		b5 FUSE_SPIEN       enable serial programming and data download (0)
		b6 FUSE_JTAGEN      enable jtag         	(0)
		b7 FUSE_OCDEN       on chip debug enable 	(1)*/

		//	/*.extebded = */ (0b11111111),
		/*
		Extended Fuse Byte
		b0 FUSE_BOOTRST     select reset vector 	(1)
		b1 FUSE_BOOTSZ0     select boot size    	(0)
		b2 FUSE_BOOTSZ1     select boot size    	(0)
		 */
};

#endif
