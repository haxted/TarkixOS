#include "../inc/gdt.h"
#include <stdint.h>
struct gdtEntry gdtTable[5];
struct gdtR gdtPtr;


void setGdtEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
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
    gdtPtr.offset = (uint64_t)&gdtTable;
    loadGdt(&gdtPtr);
    debug("Loaded GDT\n");
}

