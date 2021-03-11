#pragma once

#ifdef _DEBUG
#define MEM_DEBUG_WRAP
#endif // _DEBUG

//#include <crtdbg.h>


typedef long long alloc_size;

void* MemAllocHeap(alloc_size size, const char* Type, const char* File, int Line);
void MemFree(void* ptr);
bool LogHeap(bool group = true, bool sort = true);

#define ALLOC(Type) (Type*)MemAllocHeap(sizeof(Type)
#define ALLOC_AR(Type, len) (Type*)MemAllocHeap(sizeof(Type) * (len), #Type, __FILE__, __LINE__)
#define DEALLOC(ptr) MemFree((void*)ptr)

#define NEW(Type) new ((Type*)MemAllocHeap(sizeof(Type), #Type, __FILE__, __LINE__)) Type
#define DEL(Type, ptr) \
  ptr->~Type();        \
  MemFree((void*)ptr)
                                                                                      
