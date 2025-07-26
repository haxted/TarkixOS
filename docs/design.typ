#align(center)[
  #text(size: 24pt)[= The Design of the Tarkix Kernel]

  July 2025
]
#set heading(numbering: "1.a)")
#pagebreak()



#set text(size: 16pt)
= Introduction

This document shows the design of the Tarkix kernel.

= Kernel Architecture
The Tarkix kernel will be a 64-bit (x86_64) UNIX-like micro-kernel designed to be POSIX compliant. The kernel will be written in the C++ programming language and x86_64 Assembly.

= System Call Interface
The kernel will use the x86_64 `SYSCALL` instruction, with arguments in:
System call number: `RAX`
+ `RDI`
+ `RSI`
+ `RDX`
+ `R10`
+ `R8`
+ `R9`
and the return value of the call in `RAX`.
== System Calls
The kernel will have standard UNIX system calls, example: `ssize_t write(int fd, void* buf, size_t count);`

