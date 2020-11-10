#include "public/FrameBuff.h"
#include "public/Vec2.h"
#include <cassert>

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
  Buff = new Color4[(__int64)height * width];
  ZDepth = 0;
  parent = nullptr;
}

FBuff::~FBuff() {}

Color4* FBuff::get(SCR_UINT x, SCR_UINT y) {
  Color4* Color;
  if (!parent) {
    Color = Buff + (__int64)width * y + x;
  } else {
    Color = parent->Buff + parent->width * ((__int64)y + pos.y) + x + pos.x;
  }
  return Color;
}

void FBuff::set(SCR_UINT x, SCR_UINT y, Color4* color) {
  Color4* Color;
  if (!parent) {
    Color = Buff + (__int64)width * y + x;
  } else {
    Color = parent->Buff + parent->width * ((__int64)y + pos.y) + x + pos.x;
  }
  Color->R = color->R;
  Color->G = color->G;
  Color->B = color->B;
  Color->A = color->A;
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


void FBuff::clear() {
  SCR_UINT len = width * height;
  for (SCR_UINT i = 0; i < len; i++) {
    Buff[i].R = 0;
    Buff[i].G = 0;
    Buff[i].B = 0;
    Buff[i].A = 1.f;
  }
}

void FBuff::delBuff() { delete Buff; }

void FBuff::Resize(SCR_UINT width, SCR_UINT height) {
  delete Buff;
  this->height = height;
  this->width = width;
  Buff = new Color4[height * (__int64)width];
}

void FBuff::DrawRect(SCR_UINT x, SCR_UINT y, Color4& color, int width, int height) {
  assert((x + width <= this->width + 1) && (y + height <= this->height + 1));

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      this->set(i + x, j + y, &color);
    }
  }
}
