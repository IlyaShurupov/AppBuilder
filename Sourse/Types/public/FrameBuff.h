#pragma once

#define COLOR_RGD_32 int32_t
#define FBFF_COLOR COLOR_RGD_32

#define SET_RGB_32_R(val, ptr) *((int8_t)ptr) = val
#define SET_RGB_32_G(val, ptr) *((int8_t)ptr + 1) = val
#define SET_RGB_32_B(val, ptr) *((int8_t)ptr + 2) = val

#define SET_RGB_32_RGB(R, G, B, ptr) \
  SET_RGB_32_B(B, ptr);              \
  SET_RGB_32_G(G, ptr);              \
  SET_RGB_32_B(R, ptr);

#include "Vec2.h"

class FBuff {
 public:
  int ZDepth;
  FBFF_COLOR* Buff;

  // TODO: make use of z val & get rid of parents
  // only one level of hierarchy!!!
  vec2<SCR_UINT> pos;
  FBuff* parent;

  SCR_UINT height, width;
  FBuff();
  FBuff(SCR_UINT width, SCR_UINT height);
  ~FBuff();

  FBFF_COLOR* get(SCR_UINT x, SCR_UINT y);
  void set(SCR_UINT x, SCR_UINT y, FBFF_COLOR* color);
  void clear();
  void delBuff();
  void cast(FBuff& out, Rect<SCR_UINT>& bounds);
  void Resize(SCR_UINT width, SCR_UINT height);
  void DrawRect(SCR_UINT x, SCR_UINT y, FBFF_COLOR& color, int width, int height);

 private:
};