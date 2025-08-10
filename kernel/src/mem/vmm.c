#include "../inc/vmm.h"
#include <stddef.h>
#include <stdint.h>


extern uint64_t hhdmOffset;


void mapPage(void* pageMap, uint64_t addr, uint64_t phys, uint32_t flags) {
    printf(MSGINFO "%s:%d: PML4 is %x\n", __FILE__, __LINE__, GET_PML4(addr));
    // stolen from cavOS
    if(addr % 4096) {
        panic("Tried to map unaligned address");
    }
    uint64_t cr3 = (uint64_t)pageMap;
    int pml4i = GET_PML4(addr);
    int pdpti = GET_PDPT(addr);
    int pdi = GET_PD(addr);
    int pti = GET_PT(addr);
    uint64_t pml4Phys /* ??? */ = cr3;
    printf("VMM1\n");
    // bad code
    uint64_t* pml4 = (uint64_t*)pml4Phys;
    debug("PML4 address: ");
    printf("%x\n", (uint64_t)pml4);
        
    if(!(pml4[pml4i])) {
        size_t entry = (uint64_t)pmmAlloc();
        pml4[pml4i] = entry | PRESENT | RW | US;
    }

    

    uint64_t pml4e = pml4[pml4i];
    printf("PML4E: %x\n", pml4e);
    uint64_t pdptPhys = (pml4e & 0x000FFFFFFFFFFFFF000ULL) + hhdmOffset;
    //printf("VMM2\n");
    uint64_t* pdpt = (uint64_t*)pdptPhys;
    debug("PDPT: ");
    printf("%x\n", (uint64_t)pdptPhys);    
    if(!(pdpt[pdpti] & PRESENT)) {
        size_t* entry = pmmAlloc();
        pdpt[pdpti] = (uint64_t)entry | PRESENT | RW | US;
    }
    
    
    uint64_t pdpte = pdpt[pdpti];
    uint64_t pdPhys = (pdpte & 0x000FFFFFFFFFF000) + hhdmOffset;
    //printf("VMM3\n");
    uint64_t* pd = (uint64_t*)pdPhys;
    
    if(!(pd[pdi] & PRESENT)) {
        size_t* entry = pmmAlloc();
        pd[pdi] = (uint64_t)entry | PRESENT | RW | US;
    }
    

    uint64_t pde = pd[pdi];
    uint64_t ptPhys = (pde & 0x000FFFFFFFFFF000ULL) + hhdmOffset;
    uint64_t* pt = (uint64_t*)ptPhys;
    
    if(pt[pti] == 0) {
        pt[pti] = (uint64_t)pmmAlloc() | flags;
    }
    
    pt[pti] = phys & 0x000FFFFFFFFFF000ULL;
    //__asm__ volatile("invlpg (%0)" : : "r"((void*)addr) : "memory");
    printf(MSGINFO " Mapped virt addr %x to phys addr %x", addr, phys);
    
}
