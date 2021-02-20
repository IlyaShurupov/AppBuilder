
#pragma once

#include "Mem.h"

#define ALLOCP_NEW(Type, Allocator) new ((Type*)Allocator.Allocate(sizeof(Type), #Type, __FILE__, __LINE__)) Type
#define ALLOCP_DEL(Type, ptr, Allocator) ptr->~Type(); Allocator.Deallocate(ptr)

struct AllocatorChunck {
  static void* Allocate(alloc_size size, const char* Type, const char* File, int Line) { 
    return Alloc(size, Type, File, Line); 
  }

  static void Deallocate(void* ptr) {}
};

struct AllocatorFixedSize {
  static void* Allocate(alloc_size size, const char* Type, const char* File, int Line) { 
    return Alloc(size, Type, File, Line); 
  }

  static void Deallocate(void* ptr) {}
};

struct AllocatorSmallObj {
  static void* Allocate(alloc_size size, const char* Type, const char* File, int Line) { 
    return Alloc(size, Type, File, Line); 
  }

  static void Deallocate(void* ptr) {}
};
