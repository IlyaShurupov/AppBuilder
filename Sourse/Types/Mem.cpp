#include "public/Mem.h"

#ifdef MEM_DEBUG

#include <stdlib.h>
#include <cstdio>

struct MemHead {
  byte_size size = 0;
  MemHead* next = nullptr;
  MemHead* prev = nullptr;
};

MemHead* mem_debug_entry_ptr;

#ifdef MEM_DEBUG_WRAP

#include <cassert>

#define WRAP_TYPE int
#define WRAP_FIIL_VAL 0
#define WRAP_LEN 2  // byte / sizeof(WRAP_TYPE)

void* Alloc(byte_size size) {

  MemHead* mhptr = (MemHead*)malloc(size + sizeof(MemHead) + (WRAP_LEN * sizeof(WRAP_TYPE)) * 2);

  if (mhptr) {

    void* wrap1 = (void*)((MemHead*)mhptr + 1);
    void* wrap2 = (void*)((int*)((WRAP_TYPE*)wrap1 + WRAP_LEN) + size);

    for (byte_size i = 0; i < WRAP_LEN; i++) {
      ((WRAP_TYPE*)wrap1)[i] = WRAP_FIIL_VAL;
      ((WRAP_TYPE*)wrap2)[i] = WRAP_FIIL_VAL;
    }

    mhptr->next = mhptr->prev = nullptr;

    if (mem_debug_entry_ptr) {
      mem_debug_entry_ptr->next = mhptr;
    }

    mhptr->prev = mem_debug_entry_ptr;
    mem_debug_entry_ptr = mhptr;

    mhptr->size = size;

    return (void*)((WRAP_TYPE*)wrap1 + WRAP_LEN);
  }

  return nullptr;
}

void Free(void* ptr) {

  if (!ptr) {
    return;
  }


  MemHead* mhptr = ((MemHead*)((WRAP_TYPE*)ptr - WRAP_LEN) - 1);

  void* wrap1 = (void*)((MemHead*)mhptr + 1);
  void* wrap2 = (void*)((int*)((WRAP_TYPE*)wrap1 + WRAP_LEN) + mhptr->size);

  for (byte_size i = 0; i < WRAP_LEN; i++) {
    WRAP_TYPE val = ((WRAP_TYPE*)wrap1)[i];
    assert(val != WRAP_FIIL_VAL);
  }
  for (byte_size i = 0; i < WRAP_LEN; i++) {
    WRAP_TYPE val = ((WRAP_TYPE*)wrap2)[i];
    assert(val != WRAP_FIIL_VAL);
  }

  char padding_bytes = 1;
  char* padding_ptr = ((char*)wrap2 - padding_bytes);
  for (char i = 0; i < padding_bytes; i++) {
    padding_ptr[i] = 0;
  }

  if (mhptr->prev && mhptr->next) {
    mhptr->next->prev = mhptr->prev;
    mhptr->prev->next = mhptr->next;

  } else if (mhptr->prev || mhptr->next) {

    if (mhptr->next) {
      mhptr->next->prev = nullptr;
    } else {
      mhptr->prev->next = nullptr;
      mem_debug_entry_ptr = mhptr->prev;
    }
  }

  free(mhptr);
}

#else

void* Alloc(byte_size size) {
  MemHead* mhptr = (MemHead*)malloc(size + sizeof(MemHead));

  if (mhptr) {

    mhptr->next = mhptr->prev = nullptr;

    if (mem_debug_entry_ptr) {
      mem_debug_entry_ptr->next = mhptr;
    }

    mhptr->prev = mem_debug_entry_ptr;
    mem_debug_entry_ptr = mhptr;

    mhptr->size = size;

    return (void*)((MemHead*)mhptr + 1);
  }

  return nullptr;
}

void Free(void* ptr) {

  if (!ptr) {
    return;
  }

  MemHead* mhptr = ((MemHead*)ptr - 1);

  if (mhptr->prev && mhptr->next) {
    mhptr->next->prev = mhptr->prev;
    mhptr->prev->next = mhptr->next;

  } else if (mhptr->prev || mhptr->next) {

    if (mhptr->next) {
      mhptr->next->prev = nullptr;
    } else {
      mhptr->prev->next = nullptr;
      mem_debug_entry_ptr = mhptr->prev;
    }
  }

  free(mhptr);
}

#endif

void LogHeap() {

  if (mem_debug_entry_ptr) {

    byte_size leak_size = 0;
    MemHead* iter = mem_debug_entry_ptr;

    while (iter) {

      printf("%lli bytes at %p\n", leak_size, (iter + 1));

      leak_size += iter->size;
      iter = iter->prev;
    }

    printf("Total bytes:%lli\n", leak_size);

    return;
  }

  printf("Heap is empthy\n");
}

#endif

/*
struct AllocWrap {
  int wrapsize;
  void* buff;

  AllocWrap(void** out, int bytesize, int wrapsize = 10) {

    this->wrapsize = wrapsize;
    buff = malloc(bytesize + 2 * wrapsize * sizeof(int));

    for (int i = 0; i < wrapsize; i++) {
      buff[i] = 0;
    }

    for (int i = 0; i < wrapsize; i++) {
      buff[i] = 0;
    }
  }

  ~AllocWrap() {
    for (int i = 0; i < wrapsize; i++) {
      if (buff[i] != 0) {
        bool braked = true;
      }
    }
    delete[] buff;
  }
};
*/
