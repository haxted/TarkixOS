#include <cstdint>
#include <cstddef>
#include <limine.h>
#include "flanterm/src/flanterm.h"
#include "flanterm/src/flanterm_backends/fb.h"
#include "inc/kernel.hpp"
#include "inc/string.hpp"
#include "inc/term.hpp"
#include "inc/gdt.hpp"
#include "inc/idt.hpp"
#include "inc/pmm.hpp"
#include "inc/sched.hpp"
// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
flanterm_context *fctx;

namespace {

__attribute__((used, section(".limine_requests")))
volatile LIMINE_BASE_REVISION(3);

}

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

namespace {

__attribute__((used, section(".limine_requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

}

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .cpp file, as seen fit.

namespace {

__attribute__((used, section(".limine_requests_start")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;

}

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .cpp file.

namespace {
    __attribute__((used, section(".limine_requests")))
    volatile limine_memmap_request memmapRequest = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 3,
        .response = nullptr 
    }; 
}

namespace {
    __attribute__((used, section(".limine_requests")))
    volatile limine_hhdm_request hhdmRequest = {
        .id = LIMINE_HHDM_REQUEST,
        .revision = 0,
        .response = nullptr
    };
};

// Halt and catch fire function.
namespace {

void hcf() {
    for (;;) {
        asm ("hlt");
    }
}

}

// The following stubs are required by the Itanium C++ ABI (the one we use,
// regardless of the "Itanium" nomenclature).
// Like the memory functions above, these stubs can be moved to a different .cpp file,
// but should not be removed, unless you know what you are doing.
extern "C" {
    int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
    void __cxa_pure_virtual() { hcf(); }
    void *__dso_handle;
}

namespace kernel {
    void panic(const char *reason) {
        kernel::term::printf(MSGFAIL " Kernel panic! Reason: %s\n Regdump: \n", reason);
        std::uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15, cr2, rip;
        __asm__ volatile(
        "mov %%rax, %0\n\t"
        "mov %%rbx, %1\n\t"
        "mov %%rcx, %2\n\t"
        "mov %%rdx, %3\n\t"
        "mov %%rsi, %4\n\t"
        "mov %%rdi, %5\n\t"
        "mov %%rbp, %6\n\t"
        : "=r"(rax), "=r"(rbx), "=r"(rcx), "=r"(rdx), "=r"(rsi), "=r"(rdi), "=r"(rbp)
    );
        kernel::term::printf("RAX: %x RBX: %x RCX: %x\nRDX: %x RSI: %x RDI: %x\n RBP: %x ", rax, rbx, rcx, rdx, rsi, rdi, rbp);

        __asm__ volatile(
        "mov %%r8, %0\n\t"
        "mov %%r9, %1\n\t"
        "mov %%r10, %2\n\t"
        "mov %%r11, %3\n\t"
        "mov %%r12, %4\n\t"
        "mov %%r13, %5\n\t"
        "mov %%r14, %6\n\t"
        "mov %%cr2, %%rax\n\t"
        "mov %%rax, %7\n\t"
        "call 1f\n\t"
        "1: pop %8\n\t"
        : "=r"(r8), "=r"(r9), "=r"(r10), "=r"(r11), "=r"(r12), "=r"(r13), "=r"(r14), "=r"(cr2), "=r"(rip)
    );

        __asm__ volatile(
         "mov %%r15, %0\n\t"
         : "=m"(r15)   
        );
        kernel::term::printf("R8: %x R9: %x\n R10: %x R11: %x R12: %x\nR13: %x R14: %x R15: %x\n CR2: %x RIP: %x\n", r8, r9, r10, r11, r12, r13, r14, r15, cr2, rip);
        hcf();
    }
}



// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
extern "C" void kmain() {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Call global constructors.
    for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    
    if(memmapRequest.response == nullptr || memmapRequest.response->entry_count < 1) {
        hcf();
    }

    std::uint64_t hhdmOff = hhdmRequest.response->offset;
    
     
    std::uint32_t *fbPtr = static_cast<std::uint32_t *>(framebuffer->address);
    fctx = flanterm_fb_init(
        NULL,
        NULL,
        fbPtr, framebuffer->width, framebuffer->height, framebuffer->pitch,
        framebuffer->red_mask_size, framebuffer->red_mask_shift,
        framebuffer->green_mask_size, framebuffer->green_mask_shift,
        framebuffer->blue_mask_size, framebuffer->blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        1, 1,
        0
    );
    kernel::term::kout("\x1b[47m\x1b[30m\x1b[2J\x1b[H");
    kernel::term::kout("\x1b[34mWelcome to Tarkix, version 0.2.1\n\x1b[31mCopyright (C) 2025 haxted.\n\x1b[30mStill In Development Bozos\n");
    kernel::gdt::initGdt();
  
    // for(std::uint64_t i = 0; i < framebuffer->width*framebuffer->height*framebuffer->bpp; i++) {
    //     fbPtr[i] = i / 8 * 2;
    // }
    kernel::idt::initIdt();
    
    struct limine_memmap_response* memmapResponse = memmapRequest.response;
    kernel::term::kout(MSGOK " Got memory map from bootloader\n");
    kernel::pmm::initPmm(memmapResponse, hhdmOff);
    void* stackTop;

    void* p = kernel::pmm::pmmAlloc();
    void* p2 = kernel::pmm::pmmAlloc();
    kernel::pmm::pmmFree(p2);
    kernel::pmm::pmmFree(p);
    kernel::sched::createThread(kmain, 0, stackTop);
    kernel::panic("Test!");
    kernel::term::kout(MSGINFO" We are about to crash the system. Please be prepared.\n");
    __asm__ volatile(
        "xor %rax, %rax\n\t"
        "div %rax\n\t"
    );
    hcf();
}
