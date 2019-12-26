#pragma once

#include <stdint.h>

void putc(char c);
void puts(const char *s);
int iota(char *buf, intmax_t i, unsigned int base);
int iota_u(char *buf, uintmax_t u, unsigned int base);
void kprintf(const char *fmt, ...);