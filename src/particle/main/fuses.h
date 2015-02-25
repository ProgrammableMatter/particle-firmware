// @author: Raoul Rubien 2011

#ifndef __fuses_h__
#define __fuses_h__

#include <avr/io.h>

/**
 * Define fuse bits that are later written in the elf file.
 * The bits can be found in the .fuse section and extracted by using "avr-objcopy -j .fuse".
 * For more inforamtion see /usr/lib/avr/include/avr/fuse.h
 * also see page 295 in ATmega164/324p/664p documentation
 *
 */

/*
 Clock sources:
 CLKM1   CLKM0 of register CLKMSR
 0       0 Calibrated Internal 8 MHz Oscillator
 0       1 Internal 128 kHz Oscillator (WDT Oscillator)
 1       0 External clock
 1       1 Reserved
 */

#warning FUSES not availabe for attiny20
//FUSES =
//{
// /*.low      = */(0b11111111)
// (0) ... programmed   => on
// (1) ... unprogrammed => off
// 7 -             Reserved                         1 (unprogrammed)
// 6 BODLEVEL2 (1) Brown-out Detector trigger level 1 (unprogrammed)
// 5 BODLEVEL1 (1) Brown-out Detector trigger level 1 (unprogrammed)
// 4 BODLEVEL0 (1) Brown-out Detector trigger level 1 (unprogrammed)
// 3 -             Reserved                         1 (unprogrammed)
// 2 CKOUT System Clock Output                      1 (unprogrammed)
// 1 WDTON Watchdog Timer always on                 1 (unprogrammed)
// 0 RSTDISBL External Reset disable                1 (unprogrammed)
//};

#endif
