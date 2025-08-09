#pragma once
#include <stdint.h>
#include "term.h"
#include "../limine.h"



struct pmmFreeBlock {
    struct pmmFreeBlock* next;
};
  
void initPmm(struct limine_memmap_response* memmap, uint64_t hhdmOff);
void* pmmAlloc();
void pmmFree(void* ptr);

