#include "../inc/term.h"
#include <stdint.h>

extern struct flanterm_context *fctx;


void kout(const char* str) {
    flanterm_write(fctx, str, strlen(str));
}
void koutHex64(uint64_t val) {
    char hexDigits[] = "0123456789ABCDEF";
    kout("0x");
    char buf[17];
    for (int i = 0; i < 16; i++) {
        uint8_t nib = (val >> (60 - i * 4)) & 0xf;
        char digit = hexDigits[nib];
        buf[i] = digit;
    }
    buf[16] = '\0';
    kout(buf);
}
void koutChar(char c) {
    char chars[2];
    chars[0] = c;
    chars[1] = '\0';
    kout(chars);
}

