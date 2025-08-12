#pragma once
#include <stdint.h>
#define CPUID_VENDOR_AMD "AuthenticAMD"
#define CPUID_VENDOR_INTEL "GenuineIntel"

void __cpuid(int leaf, int* eax, int* ebx, int* ecx, int* edx);
