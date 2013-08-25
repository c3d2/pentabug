#ifndef CHIPDEF_H
#define CHIPDEF_H

#include <avr/io.h>

#if defined (SPMCSR)
#define SPM_REG SPMCSR
#elif defined (SPMCR)
#define SPM_REG SPMCR
#else
#error "AVR processor does not provide bootloader support!"
#endif

#define APP_END (FLASHEND - (BOOTSIZE * 2))

#if (SPM_PAGESIZE > UINT8_MAX)
typedef uint16_t pagebuf_t;
#else
typedef uint8_t pagebuf_t;
#endif

#if defined(__AVR_ATmega88PA__)
#include "mega88.h"

#elif defined(__AVR_ATmega88__)
#include "mega88.h"

#else
#error "no definition for MCU available in chipdef.h"
#endif

#endif
