#include "string.h"

void *memcpy(void *dst, const void *src, size_t n)
{
    char *_dst = dst;
    const char *_src = src;
    while (n) {
        *_dst++ = *_src++;
        n--;
    }
    return dst;
}

void *memmove(void *dst, const void *src, size_t n)
{
    char *_dst = dst;
    const char *_src = src;
    if (_src < _dst && _src + n > _dst) {
        while (n--) {
            *(_dst + n) = *(_src + n);
        }
        return dst;
    } else {
        return memcpy(dst, src, n);
    }
}

size_t strlen(const char *s)
{
    const char *_s = s;
    while (*_s) _s++;
    return (size_t)(_s - s);
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (int)(*s1 - *s2);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    if (n-- == 0) { return 0; }
    while (*s1 && *s2 && *s1 == *s2 && n > 0) {
        s1++;
        s2++;
        n--;
    }
    return (int)(*s1 - *s2);
}