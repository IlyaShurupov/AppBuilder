#include "public/FrameBuff.h"
#include "public/Vec2.h"
#include <cassert>

#define IDX3D(width, depth, x, y, z) (width * depth * y + depth * x + z)
#define IDX2D(width, x, y) (width * y + x)

#define BUFF_GET(x, y, width, Buff) (Buff + width * y + x)

FBuff::FBuff() {
  this->height = -1;
  this->width = -1;
  Buff = nullptr;
}

FBuff::FBuff(size_t width, size_t height) {
  this->height = height;
  this->width = width;
  Buff = new Color4[height * width];
  ZDepth = 0;
}

FBuff::~FBuff() {}

Color4* FBuff::get(size_t x, size_t y) {
  return BUFF_GET(x, y, width, this->Buff);
}

void FBuff::set(size_t x, size_t y, Color4* color) {
  Color4* Color = BUFF_GET(x, y, width, this->Buff);
  Color->R = color->R;
  Color->G = color->G;
  Color->B = color->B;
  Color->A = color->A;
}

void FBuff::cast(FBuff& out, Rect<short>& bounds) {
  //buff2.width = region->rect.width();
  //buff2.height = region->rect.height();

  //buff2.Buff = Buff->get(region->rect.v0.x, Buff->height - region->rect.v1.y);
}


void FBuff::clear() {
  size_t len = width * height;
  for (size_t i = 0; i < len; i++) {
    Buff[i].R = 0;
    Buff[i].G = 0;
    Buff[i].B = 0;
    Buff[i].A = 1.f;
  }
}

void FBuff::delBuff() { delete Buff; }

void FBuff::Resize(size_t width, size_t height) {
  delete Buff;
  this->height = height;
  this->width = width;
  Buff = new Color4[height * width];
}

void FBuff::DrawRect(size_t x, size_t y, Color4& color, int width, int height) {
  assert((x + width <= this->width + 1) && (y + height <= this->height + 1));

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      this->set(i + x, j + y, &color);
    }
  }
}
