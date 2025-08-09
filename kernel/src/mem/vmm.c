#include "../inc/vmm.h"
#include <stdint.h>


extern uint64_t hhdmOffset;

void mapPage(uint64_t addr, uint64_t phys, uint32_t flags) {
    printf(MSGINFO "%s:%d: PML4 is %x\n", __FILE__, __LINE__, GET_PML4(addr));
    uint64_t cr3;
    int pml4i = GET_PML4(addr);
    int pdpti = GET_PDPT(addr);
    int pdi = GET_PD(addr);
    int pti = GET_PT(addr);
    __asm__ volatile(
    "mov %%cr3, %0" : "=r"(cr3)
    );
    uint64_t pml4Phys = (cr3 >> 12) << 12;
    printf("VMM1\n");
    uint64_t* pml4 = (uint64_t*)pml4Phys + hhdmOffset;
    
    if(pml4[pml4i] == 0) {
        pml4[pml4i] = (uint64_t)pmmAlloc() | PRESENT | RW | US;
    }
    
    uint64_t pml4e = pml4[pml4i];
    uint64_t pdptPhys = pml4e & 0x000FFFFFFFFFF000ULL;
    printf("VMM2\n");
    uint64_t* pdpt = (uint64_t*)pdptPhys + hhdmOffset;
    
    if(pdpt[pdpti] == 0) {
        pdpt[pdpti] = (uint64_t)pmmAlloc() | PRESENT | RW | US;
    }
    
    uint64_t pdpte = pdpt[pdpti];
    uint64_t pdPhys = pdpte & 0x000FFFFFFFFFF000ULL;
    printf("VMM3\n");
    uint64_t* pd = (uint64_t*)pdPhys + hhdmOffset;

    if(pd[pdi] == 0) {
        pd[pdi] = (uint64_t)pmmAlloc() | PRESENT | RW | US;
    }

    uint64_t pde = pd[pdi];
    uint64_t ptPhys = pde & 0x000FFFFFFFFFF000ULL;
    printf("VMM4\n");
    uint64_t* pt = (uint64_t*)ptPhys + hhdmOffset;

    if(pt[pti] == 0) {
        pt[pti] = (uint64_t)pmmAlloc() | flags;
    }
    pt[pti] = phys & 0x000FFFFFFFFFF000ULL;
    printf(MSGINFO " Mapped virt addr %x to phys addr %x", addr, phys);
    
}
