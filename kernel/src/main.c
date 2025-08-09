#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "flanterm/src/flanterm.h"
#include "flanterm/src/flanterm_backends/fb.h"
#include "inc/kernel.h"
#include "inc/string.h"
#include "inc/term.h"
#include "inc/gdt.h"
#include "inc/idt.h"
#include "inc/pmm.h"
#include "inc/sched.h"
#include "inc/vmm.h"
// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
struct flanterm_context *fctx;


__attribute__((used, section(".limine_requests")))
volatile LIMINE_BASE_REVISION(3);


// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.



__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = NULL
};



// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .cpp file, as seen fit.



__attribute__((used, section(".limine_requests_start")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;



// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .cpp file.


__attribute__((used, section(".limine_requests")))
volatile struct limine_memmap_request memmapRequest = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 3,
    .response = NULL
};


__attribute__((used, section(".limine_requests")))
volatile struct limine_hhdm_request hhdmRequest = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = NULL
};

// Halt and catch fire function.


void hcf() {
    for (;;) {
        asm ("hlt");
    }
}





void _panic(const char* reason, int line, const char* file) {
    printf(MSGFAIL" %s.%d: Kernel panic! Reason: %s\n Regdump: \n", file, line, reason);
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15, cr2, rip;
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
    printf("RAX: %x RBX: %x RCX: %x\nRDX: %x RSI: %x RDI: %x\nRBP: %x ", rax, rbx, rcx, rdx, rsi, rdi, rbp);

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
    printf("R8: %x R9: %x\nR10: %x R11: %x R12: %x\nR13: %x R14: %x R15: %x\nCR2: %x RIP: %x\n", r8, r9, r10, r11, r12, r13, r14, r15, cr2, rip);
    hcf();
}




// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
    void kmain() {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    
    if(memmapRequest.response == NULL || memmapRequest.response->entry_count < 1) {
        hcf();
    }

    uint64_t hhdmOff = hhdmRequest.response->offset;
    
     
    uint32_t *fbPtr = (uint32_t*)framebuffer->address;
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
    printf("\x1b[47m\x1b[30m\x1b[2J\x1b[H");
    printf("\x1b[34mWelcome to Tarkix, version 0.3.0\n\x1b[31mCopyright (C) 2025 haxted.\n\x1b[30mStill In Development Bozos\n");
    initGdt();
  
    // for(std::uint64_t i = 0; i < framebuffer->width*framebuffer->height*framebuffer->bpp; i++) {
    //     fbPtr[i] = i / 8 * 2;
    // }
    initIdt();
    
    struct limine_memmap_response* memmapResponse = memmapRequest.response;
    debug("Got memory map from bootloader.\n");
    initPmm(memmapResponse, hhdmOff);
    void* stackTop = pmmAlloc();
    mapPage((uint64_t)initIdt, 0x7ffffffff, PRESENT | RW | US);
    printf("Better not fault\n");
    int i = *(int*)0x8000;
    void* p = pmmAlloc();
    void* p2 = pmmAlloc();
    pmmFree(p2);
    pmmFree(p);
    createThread(kmain, 0, stackTop);
    pmmFree(stackTop);
    panic("Test!");
}
