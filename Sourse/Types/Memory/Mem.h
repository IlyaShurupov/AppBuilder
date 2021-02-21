#pragma once

#ifdef _DEBUG
#define MEM_DEBUG_WRAP
#endif // _DEBUG

#include <crtdbg.h>

typedef long long alloc_size;

void* Alloc(alloc_size size, const char* Type, const char* File, int Line);
void Free(void* ptr);
bool LogHeap(bool group = true, bool sort = true);

#define ALLOC(Type) (Type*)Alloc(sizeof(Type)
#define ALLOC_AR(Type, len) (Type*)Alloc(sizeof(Type) * (len), #Type, __FILE__, __LINE__)
#define DEALLOC(ptr) Free((void*)ptr)

#define NEW(Type) new ((Type*)Alloc(sizeof(Type), #Type, __FILE__, __LINE__)) Type 
#define DEL(Type, ptr) ptr->~Type(); Free((void*)ptr)
                                                                                      
