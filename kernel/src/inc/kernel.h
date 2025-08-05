#pragma once

#define panic(reason) _panic(reason, __LINE__, __FILE__);

void _panic(const char* reason, int line, const char* file);