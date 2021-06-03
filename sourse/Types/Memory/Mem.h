#pragma once

#define MEM_DEBUG

#include <corecrt_malloc.h>

#ifdef MEM_DEBUG

void* operator new (size_t size, const char* file, int line);
void operator delete (void* p);
#define new new (__FILE__, __LINE__)

#endif
