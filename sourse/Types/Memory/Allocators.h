
#pragma once

#include "Mem.h"
#include "Macros.h"

struct AllocatorChunck {

  uint1* buff;
  uint1 first;
  uint1 freeblocks;
  uint1 blockSize;

  AllocatorChunck(uint1 p_blockSize, uint1 blocks) { 
    buff = (uint1*)MemAllocHeap(p_blockSize * blocks, "Chunck", "", 0);
    first = 0;
    freeblocks = blocks;
    blockSize = p_blockSize;

    uint1 i = 0;
    uint1* p = buff;
    for (; i != blocks; p += blockSize) {
      *p = ++i;
    }
  }

  ~AllocatorChunck() {
    MemFree(buff); 
  }

  void* Get(uint1 size) { 
    if (!freeblocks) {
      return 0;
    }
    uint1* pResult = buff + (first * blockSize);
    first = *pResult;
    freeblocks--;
    return pResult;
  }

  void Deallocate(void* ptr, alloc_size blockSize) {
    uint1* delptr = (uint1*)(ptr);
    *delptr = first;
    first = (uint1)((delptr - buff) / blockSize);
    ++freeblocks; 
  }
};

struct Chunk {
  void Init(uint4 blockSize, unsigned char blocks);
  void* Allocate(uint4 blockSize);
  void Deallocate(void* p, uint4 blockSize);
  unsigned char* pData_;
  unsigned char firstAvailableBlock_, blocksAvailable_;
}; 

struct AllocatorFixedSize { 
  static void* Get(alloc_size size) { 
    return MemAllocHeap(size, " ", " ", 1); 
  }
  static void Deallocate(void* ptr) {}
};

struct AllocatorSmallObj {
  static void* Get(alloc_size size) { 
    return MemAllocHeap(size, " ", " ", 1); 
  }
  static void Deallocate(void* ptr) {}
};

struct AllocatorGeneral {
  static void* Get(alloc_size size) { 
    return MemAllocHeap(size, " ", " ", 1); 
  }
  static void Deallocate(void* ptr) {}
};
