#include "../inc/pmm.h"
#include <stdint.h>
#include "../limine.h"


struct pmmFreeBlock* blockHead = NULL;
uint64_t hhdmOffset;

void initPmm(struct limine_memmap_response* memmap, uint64_t hhdmOff) {
    hhdmOffset = hhdmOff;
    for (uint64_t i = 0; i < memmap->entry_count; i++) {
      struct limine_memmap_entry* entry = memmap->entries[i];
      if(entry->type == LIMINE_MEMMAP_USABLE) {
        uint64_t base = (entry->base + 0xFFF) & ~0xFFFULL;
        uint64_t end = (entry->base + entry->length) & ~0xFFFULL;
        for (uint64_t addr = base; addr < end; addr += 0x1000) {
        struct pmmFreeBlock* block = (struct pmmFreeBlock*)(addr + hhdmOff);

        block->next = blockHead;
        blockHead = (struct pmmFreeBlock*)addr;
      }
    }
  }
    printf(MSGOK " Initialized Physical Memory Manager \n");
}
void* pmmAlloc() {
  if(!blockHead) return NULL;
  struct pmmFreeBlock* block = (struct pmmFreeBlock*)((uintptr_t)blockHead + hhdmOffset);
  void* addr = blockHead;
  blockHead = block->next;
  printf(MSGINFO " Allocated 4096 bytes at 0x%x\n", (uint64_t)addr);
  return addr;
}
void pmmFree(void* ptr) {
  if(!ptr) return;
  struct pmmFreeBlock* block = (struct pmmFreeBlock*)((uintptr_t)ptr + hhdmOffset);
  block->next = blockHead;
  blockHead = (struct pmmFreeBlock*)ptr;
  printf(MSGINFO " Freed memory at 0x%x\n", (uint64_t)block);
}

