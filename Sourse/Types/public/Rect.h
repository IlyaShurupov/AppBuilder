
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

  Rect(Type posx, Type posy, Type sizex, Type sizey) {
    this->pos.assign(posx, posy);
    this->size.assign(sizex, sizey);
  }

  Rect<Type>& operator=(Rect<Type>& rect) {
    pos = rect.pos;
    size = rect.size;
    return *this;
  }

  bool enclosed_in(Rect<Type>& rect) {
    return (pos.operator>(rect.pos) && rect.size.operator>(size));
  }

  bool inside(vec2<Type>& p) {
    return (p.operator>(pos) && (pos + size).operator>(p));
  }

  bool inside(Type x, Type y) {
    return (pos.x < x && pos.y < y && pos.x + size.x > x && pos.y + size.y > y);
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