#pragma once

#define CHANEL_CAPACITY float
#include "Vec2.h"

typedef struct Color4 {
  CHANEL_CAPACITY R = 0;
  CHANEL_CAPACITY G = 1.f;
  CHANEL_CAPACITY B = 0;
  CHANEL_CAPACITY A = 1.f;
} Color4;

class FBuff {
 public:
  int ZDepth;
  Color4* Buff;
  size_t height, width;
  FBuff();
  FBuff(size_t width, size_t height);
  ~FBuff();

  Color4* get(size_t x, size_t y);
  void set(size_t x, size_t y, Color4* color);
  void clear();
  void delBuff();
  void cast(FBuff& out, Rect<short>& bounds);
  void Resize(size_t width, size_t height);
  void DrawRect(size_t x, size_t y, Color4& color, int width, int height);

 private:
};