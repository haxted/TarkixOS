#include "../inc/cpuid.h"

void __cpuid(int leaf, int* eax, int* ebx, int* ecx, int* edx) {
    int a, b, c, d;
    __asm__ volatile(
        "cpuid\n\t"
        : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
        : "a"(leaf)
    );
    *eax = a;
    *ebx = b;
    *ecx = c;
    *edx = d;
}
