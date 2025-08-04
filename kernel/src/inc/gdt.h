#pragma once
#include <stdint.h>
#include "term.h"

struct gdtR {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

// struct {
//     uint32_t reserved0;
//     uint64_t rsp0;
//     uint64_t rsp1;
//     uint64_t rsp2;
//     uint64_t reserved1;
//     uint64_t ist[7];
//     uint64_t reserved2;
//     uint32_t ioPb;
// } __attribute__((packed)) tssPtr;

struct tssEntry {

};

struct gdtEntry {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t access;
    uint8_t granularity;
    uint8_t baseHigh;
} __attribute__((packed));
void setGdtEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

void initGdt();
void initTss();

extern void loadGdt(struct gdtR *gdtr);
extern void loadTss(uint16_t tssSelector);
