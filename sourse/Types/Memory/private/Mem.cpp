
#include "Memory/Mem.h"

#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include "Containers/List.h"

#ifdef MEM_DEBUG_WRAP
#include <cassert>
typedef char int1;
#define WRAP_LEN 8  // bytes
#define WRAP_FILL_VAL 1  // bytes
#endif

struct MemHead {
  alloc_size size = 0;
  MemHead* next = nullptr;
  MemHead* prev = nullptr;
  void* type = nullptr;
  void* file = nullptr;
  long long line;
};

MemHead* mem_debug_entry_ptr;


void* MemAllocHeap(alloc_size size, const char* Type, const char* File, int Line) {

#ifdef MEM_DEBUG_WRAP

  alloc_size total = size + sizeof(MemHead) + WRAP_LEN * 2;
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

    mhptr->size = size;
    mhptr->line = Line;

    int idx = 0;
    for (int i = (int)strlen(File); i > 0; i--) {
      if (File[i] == '\\') {
        idx = i;
        break;
      }
    }

    mhptr->type = new std::string(Type);
    mhptr->file = new std::string(&(File)[idx + 1]);

    #ifdef MEM_DEBUG_WRAP
    return (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN);
    #else
    return (void*)((MemHead*)mhptr + 1);
    #endif
  }

  return nullptr;
}

void MemFree(void* ptr) {

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

  } else if (mhptr->prev || mhptr->next) {

    if (mhptr->next) {
      mhptr->next->prev = nullptr;
    } else {
      mhptr->prev->next = nullptr;
      mem_debug_entry_ptr = mhptr->prev;
    }
  }

  delete (std::string*)mhptr->type;
  delete (std::string*)mhptr->file;

  free(mhptr);
}

bool LogHeap(bool group, bool sort) {

  printf("\n HEAP IN USE: \n");

  if (!mem_debug_entry_ptr) {
    printf("  Heap is empthy\n");
    return false;
  }

  alloc_size HeapSize = 0;
  MemHead* iter;

  if (!group) {
    for (iter = mem_debug_entry_ptr; iter; iter = iter->prev) {
      std::cout << "   " << ((std::string*)iter->type)->c_str() << "   " << iter->size;
      std::cout << "   " << ((std::string*)iter->file)->c_str() << "   " << iter->line << "\n";
      HeapSize += iter->size;
    }
  } else {

    typedef struct GRP {
      alloc_size size = 0;
      std::string type;
    } GRP;

    List<GRP> Groups;

    for (iter = mem_debug_entry_ptr; iter; iter = iter->prev) {
      bool in_grp = false;

      FOREACH(&Groups, GRP, i) {
        if (Groups[i].type == *(std::string*)iter->type) {
          in_grp = true;
          Groups[i].size += iter->size;
          break;
        }
      }
      if (!in_grp) {
        GRP* new_grp = NEW(GRP)();
        new_grp->size = iter->size;
        new_grp->type = *(std::string*)iter->type;
        Groups.PushBack(new_grp);
      }
    }

    Groups.Sort([](GRP& v1, GRP& v2) { return v1.size > v2.size; });
    Groups.Invert();

    FOREACH(&Groups, GRP, i) {
      std::cout << "   " << Groups[i].size << "    " << Groups[i].type.c_str() << "\n";
      HeapSize += Groups[i].size;
    }

  }


  printf("\n Total:  %lli bytes ( %f MB ) \n", HeapSize, (float)HeapSize / (1024 * 1024));
  return true;
}


