
#pragma once

#include <cassert>
#include "LinkedList.h"
#include "Rect.h"

#define RGBA_32 int32_t
#define FBFF_COLOR RGBA_32

#define COL32_A 0xff000000
#define COL32_R 0x00ff0000
#define COL32_G 0x0000ff00
#define COL32_B 0x000000ff

typedef struct RGBAf {
  float R = 1.f;
  float G = 1.f;
  float B = 1.f;
  float A = 1.f;
} RGBAf;

template <typename Color_t>
struct FBuff {

  // world buffers have their own buffers while local share one
  Hierarchy<FBuff, List<FBuff>, 0> wrld_hrchy;
  Hierarchy<FBuff, List<FBuff>, 1> local_hrchy;

  // dimentions
  vec2<SCR_UINT> size;

  // pixel array
  Color_t* pxls = nullptr;

  // z order val
  int z_depth;

  bool usealpha = false;

  // ------  cast properties ---------  //

  // position in parent fbuffer
  vec2<SCR_UINT> pos;

  // dimentions of the root fbuffer
  SCR_UINT* root_width = nullptr;
  SCR_UINT* root_height = nullptr;

  FBuff(SCR_UINT width, SCR_UINT height);
  FBuff();
  ~FBuff();

  Color_t* get(SCR_UINT x, SCR_UINT y);
  void set(SCR_UINT x, SCR_UINT y, Color_t* color);

  void resize(SCR_UINT width, SCR_UINT height);

  void coppy(FBuff<RGBAf>* fbuff);
  void cast(FBuff& out, Rect<SCR_UINT>& bounds);
  void move(SCR_UINT dx, SCR_UINT dy);

  void premultiply();

  // simple draw methods
  void DrawRect(Rect<SCR_UINT>& rect, Color_t& color);
  void clear(Color_t* color);
};

template <typename Color_t>
FBuff<Color_t>::FBuff() {
  size.assign(0, 0);
  z_depth = 0;
}

template <typename Color_t>
FBuff<Color_t>::FBuff(SCR_UINT width, SCR_UINT height) {
  size.assign(width, height);
  pxls = DBG_NEW Color_t[(__int64)height * width];
}

template <typename Color_t>
void FBuff<Color_t>::set(SCR_UINT x, SCR_UINT y, Color_t* color) {
  *get(x, y) = *color;
}

template <typename Color_t>
void FBuff<Color_t>::clear(Color_t* color) {
  long int len = size.x * size.y;
  for (int i = 0; i < len; i++) {
    pxls[i] = *color;
  }
}

template <typename Color_t>
void FBuff<Color_t>::resize(SCR_UINT width, SCR_UINT height) {
  delete pxls;
  pxls = DBG_NEW Color_t[height * (__int64)width];
  size.assign(width, height);
}

template <typename Color_t>
void FBuff<Color_t>::move(SCR_UINT dx, SCR_UINT dy) {
  pos.x += dx;
  pos.y += dy;

  FOREACH_NODE(FBuff, (&local_hrchy.childs), cld_node) { cld_node->Data->move(dx, dy); }

  FOREACH_NODE(FBuff, (&wrld_hrchy.childs), cld_node) { cld_node->Data->move(dx, dy); }
}

// NO CLAMPING
template <typename Color_t>
void FBuff<Color_t>::DrawRect(Rect<SCR_UINT>& rect, Color_t& color) {

  SCR_UINT lastpxlx = rect.pos.x + rect.size.x;
  SCR_UINT lastpxly = rect.pos.y + rect.size.y;

  for (SCR_UINT i = rect.pos.x; i < lastpxlx; i++) {
    for (SCR_UINT j = rect.pos.y; j < lastpxly; j++) {
      set(i, j, &color);
    }
  }
}

template <typename Color_t>
Color_t* FBuff<Color_t>::get(SCR_UINT x, SCR_UINT y) {

  if (!local_hrchy.parent) {
    return pxls + (__int64)size.x * y + x;
  }

  return local_hrchy.parent->pxls + *root_width * ((__int64)y + pos.y) + (x + pos.x);
}

template<typename Color_t>
void FBuff<Color_t>::coppy(FBuff<RGBAf>* fbuff) {
  resize(fbuff->size.x, fbuff->size.y);
  unsigned char r, g, b;
  SCR_UINT len = size.x * size.y;
  for (SCR_UINT i = 0; i < len; i++) {
    r = (fbuff->pxls + i)->R * 255;
    g = (fbuff->pxls + i)->G * 255;
    b = (fbuff->pxls + i)->B * 255;

    *(pxls + i) = 0xff000000 | ((RGBA_32)r << 16) | ((RGBA_32)g << 8) | (RGBA_32)b;
  }

}

template <typename Color_t>
void FBuff<Color_t>::cast(FBuff& out, Rect<SCR_UINT>& rect) {

  assert((rect.size.y <= size.y) && (rect.size.x <= size.y));

  local_hrchy.childs.add(&out);

  out.local_hrchy.parent = this;

  FBuff* root = local_hrchy.root();

  out.root_height = &root->size.y;
  out.root_width = &root->size.x;
  out.pxls = get(rect.pos.x, rect.pos.y);
}

#define RGBA_GET(color, chanel) ((unsigned char*)color + chanel)
#define RGBA_SET(color, val, chanel) *((unsigned char*)color + chanel) = val


template <typename Color_t>
void FBuff<Color_t>::premultiply() {
  SCR_UINT len = size.x * size.y;
  for (SCR_UINT i = 0; i < len; i++) {

    RGBA_32* color = (pxls + i);
    unsigned char& A = *((unsigned char*)color + 3);
    RGBA_SET(color, ((*RGBA_GET(color, 2) * A) / 255), 2);
    RGBA_SET(color, ((*RGBA_GET(color, 1) * A) / 255), 1);
    RGBA_SET(color, ((*RGBA_GET(color, 0) * A) / 255), 0);
 
  }
}

template <typename Color_t>
FBuff<Color_t>::~FBuff() {
  delete pxls;
  local_hrchy.leave();
}
