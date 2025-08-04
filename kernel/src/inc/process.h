#pragma once
#include <stdint.h>
#include <stddef.h>
#include "sys/types.h"

struct __attribute__((packed)) message {
    pid_t from;
    size_t len;
    char msg[256];
};



struct __attribute__((packed)) cpuState {
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax, rip, cs, rflags, rsp, ss, cr3;  
};

struct __attribute__((packed)) messageQueue {
    struct message messages[128];
    int head;
    int tail;
    int count;
};

typedef enum {
    S_NEW,
    S_RUNNING,
    S_SLEEP,
    S_ZOMBIE
} taskState;


struct __attribute__((packed)) process {
    pid_t pid;
    pid_t ppid;

    uid_t uid;
    gid_t gid;

    taskState state;
    int exitCode;

    struct cpuState* regs;
    struct process* children;
    struct process* siblings;
     
    struct messageQueue msgs;

    

    struct process* nextProcess;
};
