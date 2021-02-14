
#pragma once

#include "Mem.h"

struct AllocatorChunck {
  void* Allocate();
  void Free();
};

struct AllocatorFixedSize {
  void* Allocate();
  void Free();
};

struct AllocatorSmallObj {
  void* Allocate();
  void Free();
};