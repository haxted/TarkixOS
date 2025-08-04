#pragma once
#include <stdint.h>
#include "term.h"
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

typedef struct {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) idtR;
typedef struct {
    uint16_t offsetLow;
    uint16_t selector;
    uint8_t ist;
    uint8_t typeAttrs;
    uint16_t offsetMid;
    uint32_t offsetHigh;
    uint32_t reserved;
} __attribute__((packed)) idtEntry;
void idtSetGate(
    uint8_t gate,
    uint8_t ist,
    uint8_t attrs,
    uint16_t selector,
    void *handler);
void initIdt();
