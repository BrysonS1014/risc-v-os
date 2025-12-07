#ifndef KSTRING_H
#define KSTRING_H

#include "common.h"

size_t k_strlen(const char *s);
int    k_strcmp(const char *a, const char *b);
int    k_strncmp(const char *a, const char *b, size_t n);
void  *k_memset(void *dst, int c, size_t n);
void  *k_memcpy(void *dst, const void *src, size_t n);

#endif /* KSTRING_H */
