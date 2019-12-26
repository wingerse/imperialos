#include <string.h>
#include <stdarg.h>
#include "drivers/terminal/terminal.h"

void putc(char c)
{
    terminal_out_putc(c);
}

void puts(const char *s)
{
    char c;
    while ((c = *s++)) {
        putc(c);
    }
}

static int _iota(char *buf, uintmax_t i, unsigned int base)
{
    const char *table = "0123456789abcdef";
    uintmax_t n = i / base; 
    uintmax_t r = i % base;
    if (n == 0) {
        *buf = table[r];
        return 1;
    }
    int size = _iota(buf, n, base);
    buf[size] = table[r];
    return size + 1;
}

int iota(char *buf, intmax_t i, unsigned int base)
{
    uintmax_t u;
    if (i < 0) {
        *buf++ = '-';
        u = (uintmax_t)-i;
    } else {
        u = (uintmax_t)i;
    }
    int size = _iota(buf, u, base);
    buf[size] = '\0';
    return size;
}

int iota_u(char *buf, uintmax_t u, unsigned int base)
{
    int size = _iota(buf, u, base);
    buf[size] = '\0';
    return size;
}

void kprintf(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    char buf[11];
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
            case 'd': iota(buf, va_arg(va, int), 10); puts(buf); break;
            case 'u': iota_u(buf, va_arg(va, unsigned int), 10); puts(buf); break;
            case 'x': iota_u(buf, va_arg(va, unsigned int), 16); puts(buf); break;
            case 'p': iota_u(buf, va_arg(va, uintptr_t), 16); puts(buf); break;
            case 'c': putc((char)va_arg(va, int)); break;
            case 's': puts(va_arg(va, char *)); break;
            case '%': putc('%'); break;
            default:
                putc('%');
                putc(*fmt);
            }
        } else {
            putc(*fmt);
        }
        fmt++;
    }
    va_end(va);
}
