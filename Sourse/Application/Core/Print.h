#pragma once

#include <iostream>
#include "Types.h"

#define CLASS_NAME(Type) << typeid(Type).name()

enum class CMSGType {
  DEFAULT = 7,
  SUCCEED = 0x0002,
  WARNING = 14,
  ERRORtype = 0x0004,
};

inline void DBG_print(std::string str = std::string(), float val = 0) {
#ifdef _DEBUG

  if (str.length()) {
    std::cout << str;
  }
  if (val) {
    std::cout << "  " << str;
  }
  std::cout << "\n";

#endif  // _DEBUG
}

void post_MSG(CMSGType MSGtype = CMSGType::DEFAULT, std::string str = std::string(), float val = 0);

void print(Mat3f& mat);
void print(PropertyFloat& prp);
void print(PropertyInt& prp);
void print(RayHitData& HD);
void print(Ray& ray);
void print(Camera& cam);

template <typename Type>
void print(Stack<Type>& stack) {
  std::cout << "\n";
  std::cout << "Stack of " CLASS_NAME(Type) << "\n";
  std::cout << "	"
            << "Length: " << stack.Len() << "\n";
  std::cout << "	"
            << "MemSize: " << stack.MemSize() << " Bytes\n";
}

template <typename Type>
void print(vec2<Type>& ce) {
  std::cout << "\n";
  std::cout << "Vec2 " CLASS_NAME(Type) << " " << ce.x << " " << ce.y << "\n";
}

template <typename Type>
void print(List<Type>& list) {
  std::cout << "\n";
  std::cout << "Double Linked List with " << list.len() << " " CLASS_NAME(Type) << "\n";
}