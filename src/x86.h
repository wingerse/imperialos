#pragma once

#include <stdint.h>

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("in %0, %1"
        : "=a"(ret)
        :  "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile("in %0, %1"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t data)
{
    asm volatile("out %0, %1"
        : 
        : "Nd"(port), "a"(data));
}

static inline void rep_insw(uint16_t port, void *dst, int count)
{
    asm volatile("cld\n"
                 "rep insw"
                 : "+D"(dst), "+c"(count)
                 : "d"(port), "0"(dst), "1"(count)
                 : "cc", "memory");
}