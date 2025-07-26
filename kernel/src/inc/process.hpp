#pragma once
#include <cstdint>
#include "sys/types.h"

struct __attribute__((packed)) message {
    pid_t from;
    std::size_t len;
    char msg[256];
};

struct __attribute__((packed)) messageQueue {
    message messages[128];
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

    process* children;
    process* siblings;
     
    messageQueue msgs;

    

    process* nextProcess;
    
};
