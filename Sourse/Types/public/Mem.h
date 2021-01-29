#pragma once

#define MEM_DEBUG _DEBUG

typedef long long byte_size;

#ifdef MEM_DEBUG

  #include <crtdbg.h>

  #define _CRTDBG_MAP_ALLOC

  #define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
  void* Alloc(byte_size size);
  void Free(void* ptr);
  void LogHeap();

#else

  #define DBG_NEW new
  inline void* Alloc(byte_size size) { return malloc(size); }
  inline void Free(void* ptr) { free(ptr); }

#endif


#define FREE(ptr) Free((void*)ptr);

#define ALLOC_DEF(Type) Type*ptr = (Type*)Alloc(sizeof(Type));
#define ALLOC_DEF_AR(Type, len, ptr) Type*ptr = (Type*)Alloc(sizeof(Type) * (len));
#define ALLOC(Type, ptr) ptr = (Type*)Alloc(sizeof(Type));
#define ALLOC_AR(Type, len, ptr) ptr = (Type*)Alloc(sizeof(Type) * (len));