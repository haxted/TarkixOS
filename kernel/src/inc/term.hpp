#include "../flanterm/src/flanterm.h"
#include "../flanterm/src/flanterm_backends/fb.h"
#include "string.hpp"
#include <cstdint>
#define ANSIFBLACK "\x1b[30m"
#define ANSIFRED "\x1b[31m"
#define ANSIFGREEN "\x1b[32m"
#define ANSIFYELLOW "\x1b[33m"
#define ANSIFBLUE "\x1b[34m"
#define ANSIFMAGENTA "\x1b[35m"
#define ANSIFCYAN "\x1b[36m"
#define ANSIFWHITE "\x1b[37m"

#define MSGINFO ANSIFBLUE "[ INFO ]" ANSIFBLACK
#define MSGWARN ANSIFYELLOW "[ WARN ]" ANSIFBLACK
#define MSGFAIL ANSIFRED "[ FAIL ]" ANSIFBLACK
#define MSGOK ANSIFGREEN "[ OK ]" ANSIFBLACK

namespace kernel::term {
    void kout(const char* str);
    void koutHex64(std::uint64_t val);
};
