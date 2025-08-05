#include "../inc/term.h"
#include <stdarg.h>
#include <stdint.h>
#define prints(str)     kout(str); /* Your string-printing function... */
#define printhex(hex)   koutHex64(hex); /* Your hex-printing function... */
#define printdec(dec)   koutDec(dec) /* Your decimal-printing function... */
#define printchar(chara) koutChar(chara); /* Your character-printing function... */



void printf(const char *fmt, ...) {
  va_list p;
  va_start(p, fmt);
  while (*fmt != '\0') {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
      case 'd': {
        int x = va_arg(p, int);
        x = x + 1;
        x--;
        printdec(x);
        fmt++;
        break;
      }
      case 'c': {
        char c = va_arg(p, int);
        printchar(c);
        fmt++;
        break;
      }
      case 'x': {
        uint64_t x = va_arg(p, uint64_t);
        printhex(x);
        fmt++;
        break;
      }
      case 's': {
        const char *s = va_arg(p, const char *);
        if (s)
          prints(s);
        fmt++;
        break;
      }
      default: {
        printchar(*fmt);
        fmt++;
        break;
      }
      }
    } else {
      printchar(*fmt);
      fmt++;
    }
  }
  va_end(p);
}

