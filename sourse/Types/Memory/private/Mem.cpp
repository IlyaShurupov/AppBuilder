
#include "Memory/Mem.h"

#ifdef MEM_DEBUG

#include <stdlib.h>
//#include <cstdio>
//#include <iostream>
//#include <cstring>


#ifdef MEM_DEBUG_WRAP
#include <cassert>
typedef char int1;
#define WRAP_LEN 8  // bytes
#define WRAP_FILL_VAL 1  // bytes
#endif

struct MemHead {
  int size = 0;
  MemHead* next = nullptr;
  MemHead* prev = nullptr;
  void* type = nullptr;
  void* file = nullptr;
  long long line;
};

MemHead* mem_debug_entry_ptr;

#undef new

void* operator new(size_t size, const char* file, int line) {

#ifdef MEM_DEBUG_WRAP

  int total = size + sizeof(MemHead) + WRAP_LEN * 2;
  MemHead* mhptr = (MemHead*)malloc(total);

  if (mhptr) {

    for (alloc_size i = 0; i < total; i++) {
      ((int1*)mhptr)[i] = WRAP_FILL_VAL;
    }

#else

  MemHead* mhptr = (MemHead*)malloc(size + sizeof(MemHead));

  if (mhptr) {
#endif

    mhptr->next = mhptr->prev = nullptr;

    if (mem_debug_entry_ptr) {
      mem_debug_entry_ptr->next = mhptr;
    }

    mhptr->prev = mem_debug_entry_ptr;
    mem_debug_entry_ptr = mhptr;


#ifdef MEM_DEBUG_WRAP
    return (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN);
#else
    return (void*)((MemHead*)mhptr + 1);
#endif
  }

  return nullptr;
}

void operator delete (void* ptr) {
  if (!ptr) {
    return;
  }

#ifdef MEM_DEBUG_WRAP

  MemHead* mhptr = ((MemHead*)((int1*)ptr - WRAP_LEN) - 1);

  void* wrap1 = (void*)((MemHead*)mhptr + 1);
  void* wrap2 = (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN + mhptr->size);

  for (alloc_size i = 0; i < WRAP_LEN; i++) {
    int1 val1 = ((int1*)wrap1)[i];
    int1 val2 = ((int1*)wrap2)[i];
    assert(val1 == WRAP_FILL_VAL && val2 == WRAP_FILL_VAL);
  }

#else
  MemHead* mhptr = ((MemHead*)ptr - 1);
#endif

  if (mhptr->prev && mhptr->next) {
    mhptr->next->prev = mhptr->prev;
    mhptr->prev->next = mhptr->next;

  }
  else if (mhptr->prev || mhptr->next) {

    if (mhptr->next) {
      mhptr->next->prev = nullptr;
    }
    else {
      mhptr->prev->next = nullptr;
      mem_debug_entry_ptr = mhptr->prev;
    }
  }

  free(mhptr);
}

#endif