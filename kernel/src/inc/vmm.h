#pragma once
#include <stdint.h>
#include "term.h"
#include "pmm.h"
#include "kernel.h"

#define PRESENT (1ULL << 0)
#define RW      (1ULL << 1)
#define US      (1ULL << 2)
#define PWT     (1ULL << 3)
#define PCD     (1ULL << 4)
#define D       (1ULL << 6)


#define GET_PML4(addr) (addr >> 39) & 0x1FF
#define GET_PDPT(addr) (addr >> 30) & 0x1FF
#define GET_PD(addr) (addr >> 21) & 0x1FF
#define GET_PT(addr) (addr >> 12) & 0x1FF
#define GET_OFF(addr) (addr & 0xFFF)

void mapPage(void* pageMap, uint64_t addr, uint64_t phys, uint32_t flags);
uint64_t virtToPhys(uint64_t virt);
