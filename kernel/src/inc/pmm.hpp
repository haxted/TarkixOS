#include <cstdint>
#include "term.hpp"
#include "../limine.h"


namespace kernel::pmm {
  struct pmmFreeBlock {
    pmmFreeBlock* next;
  };
  
  void initPmm(struct limine_memmap_response* memmap, std::uint64_t hhdmOff);
  void* pmmAlloc();
  void pmmFree(void* ptr);
}
