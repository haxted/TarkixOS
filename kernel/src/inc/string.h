#pragma once
#include <stdint.h>
#include <stddef.h>


size_t strlen(const char* str);
int memcmp(const void *s1, const void *s2, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memcpy(void *__restrict dest, const void *__restrict src, size_t n);
void *memset(void *s, int c, size_t n);
