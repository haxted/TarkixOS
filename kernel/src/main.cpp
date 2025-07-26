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
        .revision = 0,
        .response = nullptr 
    }; 
}

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
    kernel::term::kout("\x1b[34mWelcome to PlanTark, version 0.0.1\n\x1b[31mCopyright (C) 2025 haxted.\n\x1b[30mStill In Development Bozos\n");
    kernel::gdt::initGdt();
  
    // for(std::uint64_t i = 0; i < framebuffer->width*framebuffer->height*framebuffer->bpp; i++) {
    //     fbPtr[i] = i / 8 * 2;
    // }
    kernel::idt::initIdt();


    struct limine_memmap_response* memmapResponse = memmapRequest.response;

    for(std::uint64_t i = 0; i < memmapResponse->entry_count; i++) {
        struct limine_memmap_entry* entry = memmapResponse->entries[i];
        if(entry->type == 0) {
          kernel::term::kout(MSGINFO " Found free memmap entry. Base ");
          kernel::term::koutHex64(entry->base);
          kernel::term::kout(" Length ");
          kernel::term::koutHex64(entry->length);
          kernel::term::kout("\n");
        }
        
    }
    kernel::term::kout(MSGINFO" We are about to crash the system. Please be prepared.\n");
    __asm__ volatile(
        "xor %rax, %rax\n\t"
        "div %rax\n\t"
    );
    hcf();
}
