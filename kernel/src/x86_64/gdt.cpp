#include "../inc/gdt.hpp"
kernel::gdt::gdtEntry gdtTable[5];
kernel::gdt::gdtR gdtPtr;

namespace kernel::gdt {
    void setGdtEntry(int index, std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::uint8_t granularity) {
        gdtTable[index].limitLow = (limit & 0xFFFF);
        gdtTable[index].baseLow = (base & 0xFFFF);
        gdtTable[index].baseMiddle = (base >> 16) & 0xFF;
        gdtTable[index].access = access;
        gdtTable[index].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
        gdtTable[index].baseHigh = (base >> 24) & 0xFF;
    }
    void initGdt() {
        setGdtEntry(0, 0, 0, 0, 0); // Null desc
        setGdtEntry(1, 0, 0, 0b10011011, 0b10100000);
        setGdtEntry(2, 0, 0, 0b10010011, 0b11000000);
        setGdtEntry(3, 0, 0, 0b11111011, 0b10100000);
        setGdtEntry(4, 0, 0, 0b11110011, 0b11000000);

        gdtPtr.size = sizeof(gdtTable) - 1;
        gdtPtr.offset = (std::uint64_t)&gdtTable;
        loadGdt(&gdtPtr);
        kernel::term::kout("\x1b[32m[ OK ]\x1b[30m Loaded GDT\n");
    }
};
