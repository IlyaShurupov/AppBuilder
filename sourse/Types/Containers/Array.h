# pragma once

#include "Memory/Allocators.h"
#include "Macros.h"

template <typename Type, typename Allocator = AllocatorGeneral>
class Array {
  
  Type* buff;
  uint4 length;
  Allocator alloc;

 public:

  Array() { 
    length = 0;
  }

  Array(uint4 p_length) { 
    length = p_length;
    Reserve(length);
  }
  
  uint4 Len() {
    return length;
  }

  void Reserve(uint4 p_bufflen) {
    length = p_bufflen;
    buff = (Type*)alloc.Get(sizeof(Type) * length); 
  }

  void Free() {
    length = 0;
    alloc.Deallocate(buff);
    buff = nullptr;
  }

  void Insert(Type& p_block, uint4 idx) {
    Type* current = buff;
    Reserve(length + 1);

    for (uint4 befor = 0; befor < idx; befor++) {
      buff[befor] = current[befor];
    }
    for (uint4 after = idx; after < length; after++) {
      buff[after + 1] = current[after];
    }

    buff[idx] = p_block;

    alloc.Deallocate(current);
  }

  void Remove(uint4 p_idx) {
    Type* current = buff;
    Reserve(length - 1);

    for (uint4 befor = 0; befor < p_idx; befor++) {
      buff[befor] = current[befor];
    }
    for (uint4 after = p_idx; after < length + 1; after++) {
      buff[after - 1] = current[after];
    }

    alloc.Deallocate(current);
  }

  Type& operator[](uint4 idx) {
    return buff[idx];
  }

  void PushBack(Type block) {
    Insert(block, length);
  }

  ~Array() {
  }
};