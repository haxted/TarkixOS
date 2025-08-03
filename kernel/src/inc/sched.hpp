#pragma once
#include "process.hpp"
#include <cstdint>

namespace kernel::sched {
  process* createThread(
    void (*entry)(),
    std::uint64_t cr3,
    void* stackTop   
  );
  void queueThread(process* p);
  void dequeueThread(process* p);
}
