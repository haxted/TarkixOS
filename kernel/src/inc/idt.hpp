#pragma once
#include <cstdint>
#include "term.hpp"
#define DIVBYZERO 0
#define DBGEXCEPTION 1
#define NONMASKINT 2
#define BREAKPOINTEXCP 3
#define OVERFLOWEXCP 4
#define BOUNDRANGEEXCP 5 
#define UNDEFINEDOPCODE 6
#define DEVICENOTAVLBL 7
#define DOUBLEFAULT 8
#define RESERVED1 9
#define INVALIDTSS 10
#define SEGMENTNOTPRESENT 11
#define SSFAULT 12
#define GENERALPROTFAULT 13
#define PAGEFAULT 14
#define RESERVED2 15
#define X87FPERROR 16
#define ALIGNCHECK 17
#define MACHINECHECK 18
#define SIMDFPERROR 19
#define VIRTEXCP 20
#define CONTROLPROTEXCP 21

namespace kernel::idt {
    typedef struct {
        std::uint16_t size;
        std::uint64_t offset;
    } __attribute__((packed)) idtR;
    typedef struct {
        std::uint16_t offsetLow;
        std::uint16_t selector;
        std::uint8_t ist;
        std::uint8_t typeAttrs;
        std::uint16_t offsetMid;
        std::uint32_t offsetHigh;
        std::uint32_t reserved;
    } __attribute__((packed)) idtEntry;
    void idtSetGate(
        std::uint8_t gate,
        std::uint8_t ist,
        std::uint8_t attrs,
        std::uint16_t selector,
        void* handler
    );
    void initIdt();
}
