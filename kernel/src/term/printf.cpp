#include "../inc/term.hpp"
#include <cstdarg>
#include <cstdint>
#define prints(str)     kernel::term::kout(str); /* Your string-printing function... */
#define printhex(hex)   kernel::term::koutHex64(hex); /* Your hex-printing function... */
#define printdec(dec)   /* Your decimal-printing function... */
#define printchar(chara) kernel::term::koutChar(chara); /* Your character-printing function... */

namespace kernel::term {

void printf(const char *fmt, ...) {
  va_list p;
  va_start(p, fmt);
  while (*fmt != '\0') {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
      case 'd': {
        int x = va_arg(p, int);
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
        int x = va_arg(p, std::uint64_t);
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
} // namespace kernel::term
