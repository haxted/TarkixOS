#pragma once
#include <cstdint>
#include "term.hpp"



namespace kernel::gdt {
 
    struct gdtR {
        std::uint16_t size;
        std::uint64_t offset;
    } __attribute__((packed));

    struct {
        std::uint32_t reserved0;
        std::uint64_t rsp0;
        std::uint64_t rsp1;
        std::uint64_t rsp2;
        std::uint64_t reserved1;
        std::uint64_t ist[7];
        std::uint64_t reserved2;
        std::uint32_t ioPb;
    } __attribute__((packed)) tssPtr;

    struct tssEntry {
        
    };
    
    struct gdtEntry {
        std::uint16_t limitLow;
        std::uint16_t baseLow;
        std::uint8_t baseMiddle;
        std::uint8_t access;
        std::uint8_t granularity;
        std::uint8_t baseHigh;
    } __attribute__((packed));
    void setGdtEntry(int index, std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::uint8_t granularity);
    
    void initGdt();
    void initTss();
}
extern "C" void loadGdt(kernel::gdt::gdtR* gdtr);
extern "C" void loadTss(std::uint16_t tssSelector);
