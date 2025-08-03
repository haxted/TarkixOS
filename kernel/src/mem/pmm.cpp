#include "../inc/pmm.hpp"
#include <cstdint>

namespace kernel::pmm {
  pmmFreeBlock* blockHead = nullptr;
  std::uint64_t hhdmOffset;

  void initPmm(struct limine_memmap_response *memmap, std::uint64_t hhdmOff) {
    hhdmOffset = hhdmOff;
    for(std::uint64_t i = 0; i < memmap->entry_count; i++) {
      limine_memmap_entry* entry = memmap->entries[i];
      if(entry->type == LIMINE_MEMMAP_USABLE) {
      std::uint64_t base = (entry->base + 0xFFF) & ~0xFFFULL;
      std::uint64_t end = (entry->base + entry->length) & ~0xFFFULL;
      for(std::uint64_t addr = base; addr < end; addr += 0x1000) {
        pmmFreeBlock* block = (pmmFreeBlock*)(addr + hhdmOff);

        block->next = blockHead;
        blockHead = (pmmFreeBlock*)addr;
      }
    }
  }
  kernel::term::kout(MSGOK " Initialized Physical Memory Manager \n");
}
  void* pmmAlloc() {
    if(!blockHead) return nullptr;
    pmmFreeBlock* block = (pmmFreeBlock*)((std::uintptr_t)blockHead + hhdmOffset);
    void* addr = blockHead;
    blockHead = block->next;
    kernel::term::printf(MSGINFO " Allocated 4096 bytes at 0x%x\n", (std::uint64_t)addr);
    return addr;
  }
  void pmmFree(void* ptr) {
    if(!ptr) return;
    pmmFreeBlock* block = (pmmFreeBlock*)((std::uintptr_t)ptr + hhdmOffset);
    block->next = blockHead;
    blockHead = (pmmFreeBlock*)ptr;
    kernel::term::printf(MSGINFO " Freed memory at 0x%x\n", (std::uint64_t)block);
  }
}
