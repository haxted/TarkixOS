#include "../inc/idt.hpp"
#include "../inc/kernel.hpp"
static kernel::idt::idtR idtr;
kernel::idt::idtEntry idtTable[256];
extern void* isrStubTable[];

namespace kernel::idt {
    void idtSetGate(
        std::uint8_t gate,
        std::uint8_t ist,
        std::uint8_t attrs,
        std::uint16_t selector,
        void* handler
    ) {
        idtTable[gate].offsetLow = (std::uint64_t)handler & 0xffff;
        idtTable[gate].selector = selector;
        idtTable[gate].ist = ist;
        idtTable[gate].typeAttrs = attrs;
        idtTable[gate].offsetMid = ((std::uint64_t) handler >> 16) & 0xffff;
        idtTable[gate].offsetHigh = ((std::uint64_t) handler >> 32) & 0xffffffff;
        idtTable[gate].reserved = 0;
    }
    void initIdt() {
        idtr.size = sizeof(idtTable) - 1;
        idtr.offset = (std::uint64_t)&idtTable;
        for(std::uint8_t i = 0; i < 32; i++) {
            idtSetGate(i, 0, 0x8e, 0x08, isrStubTable[i]);
        }
        __asm__ volatile(
            "lidt %0\n\t"
            "sti"
            :
            : "m"(idtr)
        );
        kernel::term::kout(MSGOK" Initialized IDT\n");
    }

}

__attribute__((noreturn))
extern "C" void exceptionHandle(std::uint64_t intno, std::uint64_t errcode, std::uint64_t rip, std::uint64_t cs, std::uint64_t rflags) {
   (void)errcode;
   (void)intno;
   switch(intno) {
    case UNDEFINEDOPCODE: {
        kernel::term::kout(MSGFAIL" Invalid opcode exception\nHalting system..");
        __asm__ volatile("cli; hlt");
        break;
    }
    case DIVBYZERO: {
        kernel::term::kout(MSGFAIL" Division by zero error\nHalting system..");
        __asm__ volatile("cli; hlt");
    }
    case PAGEFAULT: {
        std::uint64_t cr2;
        __asm__ volatile(
            "mov %%cr2, %0\n\t"
            : "=r"(cr2)
        );
        kernel::term::printf("[%s] -- Page fault --\n faultingRIP: %x\n", __func__, rip);
        kernel::panic("exceptions: Page fault");        
    }
    } 
}
