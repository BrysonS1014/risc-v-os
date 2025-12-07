#include "kstring.h"

size_t k_strlen(const char *s)
{
    size_t n = 0;
    if (!s) return 0;
    while (*s++) n++;
    return n;
}

int k_strcmp(const char *a, const char *b)
{
    if (a == b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

int k_strncmp(const char *a, const char *b, size_t n)
{
    if (n == 0) return 0;
    if (!a) return -1;
    if (!b) return 1;
    while (n-- && *a && (*a == *b)) {
        if (n == 0) return 0;
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

void *k_memset(void *dst, int c, size_t n)
{
    unsigned char *p = (unsigned char *)dst;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return dst;
}

void *k_memcpy(void *dst, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    while (n--) {
        *d++ = *s++;
    }
    return dst;
}
