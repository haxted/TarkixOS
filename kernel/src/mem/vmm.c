#include "../inc/vmm.h"
#include <stddef.h>
#include <stdint.h>
#include "../inc/cpuid.h"

extern uint64_t hhdmOffset;


void mapPage(void* pageMap, uint64_t addr, uint64_t phys, uint32_t flags) {
    int eax = 0, ebx = 0, ecx = 0, edx = 0;
    __cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
    uint64_t mask = MASK(eax & 0xff);
    // stolen from cavOS
    uint64_t bruhuhhhh = addr;
    if(bruhuhhhh % 4096) {
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
        
    if(!(pml4[pml4i])) {
        size_t entry = (uint64_t)pmmAlloc();
        pml4[pml4i] = entry | PRESENT | RW | US;
    }

    

    uint64_t pml4e = pml4[pml4i];
    printf("PML4E: %x\n", pml4e);
    uint64_t pdptPhys = (pml4e & mask) + hhdmOffset;
    //printf("VMM2\n");
    uint64_t* pdpt = (uint64_t*)pdptPhys;
    if(!(pdpt[pdpti] & PRESENT)) {
        size_t* entry = pmmAlloc();
        pdpt[pdpti] = (uint64_t)entry | PRESENT | RW | US;
    }
    
    
    uint64_t pdpte = pdpt[pdpti];
    uint64_t pdPhys = (pdpte & mask) + hhdmOffset;
    //printf("VMM3\n");
    uint64_t* pd = (uint64_t*)pdPhys;
    
    if(!(pd[pdi] & PRESENT)) {
        size_t* entry = pmmAlloc();
        pd[pdi] = (uint64_t)entry | PRESENT | RW | US;
    }
    

    uint64_t pde = pd[pdi];
    uint64_t ptPhys = (pde & mask) + hhdmOffset;
    uint64_t* pt = (uint64_t*)ptPhys;
    
    if(!(pt[pti] & PRESENT)) {
        pt[pti] = (uint64_t)pmmAlloc() | flags;
    }
    
    pt[pti] = phys & mask;
    //__asm__ volatile("invlpg (%0)" : : "r"((void*)addr) : "memory");
    printf(MSGINFO " Mapped virt addr %x to phys addr %x\n", addr, phys);
    
}


uint64_t virtToPhys(void* pageMap, uint64_t virt) {
    int pml4i = GET_PML4(virt);
    int pdpti = GET_PDPT(virt);
    int pdi = GET_PD(virt);
    int pti = GET_PT(virt);

    int eax, ebx, ecx, edx = 0;
    __cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
    uint64_t mask = MASK(eax & 0xff);
    

    uint64_t* pml4 = (uint64_t*)pageMap;
    if(!((pml4[pml4i]) & PRESENT)) {
        size_t* entry = pmmAlloc();
        pml4[pml4i] = (uint64_t)entry | PRESENT | RW | US;
    }
    uint64_t pml4e = pml4[pml4i] & mask;
    uint64_t pdptIThink = pml4e + hhdmOffset;
    uint64_t* pdpt = (uint64_t*)pdptIThink;

    if(!((pdpt[pdpti]) & PRESENT)) {
        size_t* entry = pmmAlloc();
        pdpt[pdpti] = (uint64_t)entry | PRESENT | RW | US;
    }

    uint64_t pdpte = pdpt[pdpti] & mask;
    uint64_t pdWhatIsHappening = pdpte + hhdmOffset;
    uint64_t* pd = (uint64_t*)pdWhatIsHappening;

    if(!(pd[pdi] & PRESENT)) {
        size_t* entry = pmmAlloc();
        pd[pdi] = (uint64_t)entry | PRESENT | RW | US;
    }

    uint64_t pde = pd[pdi] & mask;
    uint64_t ptSendHelpNow = pde + hhdmOffset;
    uint64_t* pt = (uint64_t*)ptSendHelpNow;

    if (!(pt[pti] & PRESENT)) {
        return 0;
    }
    uint64_t physAddr = pt[pti] & mask;
    return physAddr + GET_OFF(virt);
}
