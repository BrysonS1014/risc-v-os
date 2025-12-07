#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

void panic(const char *msg) __attribute__((noreturn));

#endif /* COMMON_H */
