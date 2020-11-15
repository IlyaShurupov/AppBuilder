#include "public/FrameBuff.h"
#include <cassert>
#include "public/Vec2.h"

#define IDX3D(width, depth, x, y, z) (width * depth * y + depth * x + z)
#define IDX2D(width, x, y) (width * y + x)

#define BUFF_GET(x, y, fBuff) (fBuff->Buff + (__int64)fBuff->pitch * y + x)

FBuff::FBuff() {
  this->height = -1;
  this->width = -1;
  Buff = nullptr;
  ZDepth = 0;
  parent = nullptr;
}

FBuff::FBuff(SCR_UINT width, SCR_UINT height) {
  this->height = height;
  this->width = width;
  Buff = new FBFF_COLOR[(__int64)height * width];
  ZDepth = 0;
  parent = nullptr;
}

FBuff::~FBuff() {}

FBFF_COLOR* FBuff::get(SCR_UINT x, SCR_UINT y) {
  FBFF_COLOR* Color;
  if (!parent) {
    Color = Buff + (__int64)width * y + x;
  } else {
    Color = parent->Buff + parent->width * ((__int64)y + pos.y) + x + pos.x;
  }
  return Color;
}

void FBuff::set(SCR_UINT x, SCR_UINT y, FBFF_COLOR* color) {
  FBFF_COLOR* Color;
  if (!parent) {
    Color = Buff + (__int64)width * y + x;
  } else {
    Color = parent->Buff + parent->width * ((__int64)y + pos.y) + x + pos.x;
  }
  return;
}

void FBuff::cast(FBuff& out, Rect<SCR_UINT>& bounds) {
  assert((bounds.height() <= this->height) && (bounds.width() <= this->width));

  out.parent = this;
  out.pos.assign(bounds.v0.x, this->height - bounds.v1.y);
  out.width = bounds.width();
  out.height = bounds.height();
  out.Buff = get(bounds.v0.x, this->height - bounds.v1.y);
}

void FBuff::clear(FBFF_COLOR* color) {
  int len = width * height;
  for (int i = 0; i < len; i++) {
    Buff[i] = *color;
  }
}

void FBuff::delBuff() {
  delete Buff;
}

void FBuff::Resize(SCR_UINT width, SCR_UINT height) {
  delete Buff;
  this->height = height;
  this->width = width;
  Buff = new FBFF_COLOR[height * (__int64)width];
}

void FBuff::DrawRect(SCR_UINT x, SCR_UINT y, FBFF_COLOR& color, int width, int height) {
  
  ((x + width <= this->width + 1) && (y + height <= this->height + 1));

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      this->set(i + x, j + y, &color);
    }
  }
}
