#include <stdlib.h>
#include <iostream>
#include <cassert>


size_t getSize();
bool CheckLeaks(bool Error);
inline void* DebugAlloc(size_t size, std::string* str);
inline void MemFree(void* ptr);
inline void* MemAlloc(size_t size);