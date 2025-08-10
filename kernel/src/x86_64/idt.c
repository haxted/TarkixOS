#include "../inc/idt.h"
#include "../inc/kernel.h"
static idtR idtr;
idtEntry idtTable[256];
extern void* isrStubTable[];


void idtSetGate(
    uint8_t gate,
    uint8_t ist,
    uint8_t attrs,
    uint16_t selector,
    void* handler
) {
    idtTable[gate].offsetLow = (uint64_t)handler & 0xffff;
    idtTable[gate].selector = selector;
    idtTable[gate].ist = ist;
    idtTable[gate].typeAttrs = attrs;
    idtTable[gate].offsetMid = ((uint64_t)handler >> 16) & 0xffff;
    idtTable[gate].offsetHigh = ((uint64_t)handler >> 32) & 0xffffffff;
    idtTable[gate].reserved = 0;
}
void initIdt() {
    idtr.size = sizeof(idtTable) - 1;
    idtr.offset = (uint64_t)&idtTable;
    for (uint8_t i = 0; i < 32; i++) {
        idtSetGate(i, 0, 0x8e, 0x08, isrStubTable[i]);
    }
    __asm__ volatile(
        "lidt %0\n\t"
        "sti"
        :
    : "m"(idtr)
        );
    debug("Initialized IDT\n");
}



__attribute__((noreturn))
void exceptionHandle(uint64_t intno, uint64_t errcode, uint64_t rip, uint64_t cs, uint64_t rflags) {
    (void)errcode;
    (void)intno;
    switch (intno) {
    case UNDEFINEDOPCODE: {
        if(cs == 0x08) {
            debug("Invalid opcode exception\nHalting system..");
            __asm__ volatile("cli; hlt");
        }
        break;
    }
    case DIVBYZERO: {
        if(cs == 0x08) {
            debug(" -- Division by zero exception --\n");
            printf("RIPdiv: %x\n", rip);
            panic("exceptions: Division by zero");

        }
    }
    case PAGEFAULT: {
        uint64_t cr2;
        __asm__ volatile(
            "mov %%cr2, %0\n\t"
            : "=r"(cr2)
            );
        if(cs == 0x08) {
            debug(" -- Page fault exception -- ");
            printf("Error code: %x\n", errcode);
            panic("exceptions: Page fault");
        }
    case GENERALPROTFAULT: {
            if(cs == 0x08) {
                debug(" -- #GP exception -- \n");
                printf("Error code: %x\nCS: %x\nRIP: %x\n", errcode, cs, rip);
                panic("exceptions: #GP")
            }
        }
    }
    }
}
