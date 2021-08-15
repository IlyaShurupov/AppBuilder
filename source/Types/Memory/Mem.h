#pragma once

#include <cstddef>

void* operator new (size_t size, const char* file, int line);
void operator delete (void* p);
void operator delete (void* p, const char* file, int line);
#define new new (__FILE__, __LINE__)

int mem_allocated_size();

//bool init_mem_debug();
