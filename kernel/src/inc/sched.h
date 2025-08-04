#pragma once
#include "process.h"
#include <stdint.h>


struct process* createThread(
    void (*entry)(),
    uint64_t cr3,
    void* stackTop   
);
void queueThread(struct process* p);
void dequeueThread(struct process* p);

