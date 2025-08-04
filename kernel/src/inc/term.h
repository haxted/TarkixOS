#include "../flanterm/src/flanterm.h"
#include "../flanterm/src/flanterm_backends/fb.h"
#include "string.h"
#include <stdint.h>
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


void kout(const char* str);
void koutHex64(uint64_t val);
void printf(const char* fmt, ...);
void koutDec64(int64_t val);
void koutChar(char c);

