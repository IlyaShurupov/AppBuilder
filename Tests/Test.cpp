
#include "Types.h"

int main() {
  AllocatorGeneral aloc;
  void* g = (void*)aloc.Get(4);
}