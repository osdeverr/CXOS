/******************************************************************************
 *
 * Name: accxos.h - OS specific defines, etc. for CXOS
 *
 *****************************************************************************/

#ifndef __ACCXOS_H__
#define __ACCXOS_H__

#include <stdarg.h>

#define ACPI_MACHINE_WIDTH 32

// Don't use the full debugger, only the features for debugging output
#define ACPI_DEBUG_OUTPUT

// Let APCICA use its own cache
#define ACPI_USE_LOCAL_CACHE

// Limit compatibility to ACPI 5.0
#define ACPI_REDUCED_HARDWARE TRUE

/*
 * Math helper asm macros
 */
#define ACPI_DIV_64_BY_32(n_hi, n_lo, d32, q32, r32) \
    asm("divl %2;"                   \
                   : "=a"(q32), "=d"(r32)           \
                   : "r"(d32),                  \
                    "0"(n_lo), "1"(n_hi))

#define ACPI_MUL_64_BY_32(n_hi, n_lo, d32, q32, r32) \
    asm("mull %2;"                   \
                    : "=a"(q32), "=d"(r32)           \
                    : "r"(d32),                  \
                     "0"(n_lo), "1"(n_hi))

#define ACPI_SHIFT_RIGHT_64(n_hi, n_lo) \
    asm("shrl   $1,%2   ;"  \
                   "rcrl   $1,%3;"     \
                   : "=r"(n_hi), "=r"(n_lo)    \
                   : "0"(n_hi), "1"(n_lo))


#endif /* __ACCXOS_H__ */
