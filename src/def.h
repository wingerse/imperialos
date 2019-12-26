#pragma once

#include <stdint.h>

// linker symbols - must be accesses as addresses only
extern char L_KERNEL_START;
extern char L_BSS_START;
extern char L_BSS_SIZE;
extern char L_KERNEL_SIZE;

#define KERNEL_START ((uintptr_t)&L_KERNEL_START)
#define BSS_START    ((uintptr_t)&L_BSS_START)
#define BSS_SIZE     ((uintptr_t)&L_BSS_SIZE)
#define KERNEL_SIZE  ((uintptr_t)&L_KERNEL_SIZE)

#define MAX(a, b)      ((a) > (b) ? (a) : (b))
#define MIN(a, b)      ((a) < (b) ? (a) : (b))
#define ROUND_UP(v, r) ((((v)+(r)-1)/(r)) * (r))
#define ALIGN(v, a)    (((v)+(a)-1) & ~((a)-1))