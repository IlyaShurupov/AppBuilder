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
  SCR_UINT height, width;
  FBuff();
  FBuff(SCR_UINT width, SCR_UINT height);
  ~FBuff();

  Color4* get(SCR_UINT x, SCR_UINT y);
  void set(SCR_UINT x, SCR_UINT y, Color4* color);
  void clear();
  void delBuff();
  void cast(FBuff& out, Rect<SCR_UINT>& bounds);
  void Resize(SCR_UINT width, SCR_UINT height);
  void DrawRect(SCR_UINT x, SCR_UINT y, Color4& color, int width, int height);

 private:
};