
#pragma once

#include "Vec2.h"


template <typename Type>
struct Rect {

  vec2<Type> pos, size;

  Rect() {}

  Rect(vec2<Type>& pos, vec2<Type>& size) {
    this->pos = pos;
    this->size = size;
  }

  Rect<Type>& operator=(Rect<Type>& rect) {
    pos = rect.pos;
    size = rect.size;
    return *this;
  }

  inline vec2<Type> size_vec() { 
    return vec2<Type>(size.x, size.y); 
  }

  inline vec2<Type> size_vec_w() {
    return vec2<Type>(size.x + pos.x, size.y + pos.y);
  }

  void inv_y(Type scr_y) {
    pos.y = scr_y - pos.y - size.y;
  }

  void move(Type& dx, Type& dy) {
    pos.x += dx;
    pos.y += dy;
  }
};

template <typename Type>
struct Edge {
  vec2<Type> v1, v2;
};