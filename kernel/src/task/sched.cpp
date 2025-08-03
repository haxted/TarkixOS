#include "../inc/sched.hpp"
#include "../inc/term.hpp"

namespace kernel::sched {
  process* currentProcess = nullptr;
  pid_t currentPid = 0;
  taskState s;
  process* createThread(void (*entry)(), std::uint64_t cr3, void *stackTop) {
    process _p = {0};
    cpuState r = {0};
    process* p = &_p;
    p->regs = &r;
    p->pid = currentPid+1;
    p->uid = p->gid = 0;
    p->regs->r15 = p->regs->r14 = p->regs->r13 = p->regs->r12 = 0;
    p->regs->r11 = p->regs->r10 = p->regs->r9 = p->regs->r8 = 0;
    p->regs->rsi = p->regs->rdi = p->regs->rdx = p->regs->rcx = 0;
    p->regs->rbx = p->regs->rax = 0;
    p->regs->rip = (std::size_t)entry;
    p->regs->cs = 0x18; // User code segment
    p->regs->rflags = 0;
    p->regs->rsp = (std::size_t)stackTop;
    p->regs->ss = 0x1b; // User data segment
    p->regs->rbp = p->regs->rsp;
    p->regs->cr3 = cr3;

    p->taskState = S_NEW; 

    currentProcess = p;
    currentProcess->siblings = p->nextProcess;

    kernel::term::printf(MSGINFO " Created process with PID %x\n", (std::uint64_t)p->pid);
    
    return p;
  }
}
