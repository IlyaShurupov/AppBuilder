
#pragma once

#include "Hierarchy.h"
#include "LinkedList.h"
#include "Rect.h"

#define RGBA_32 int
#define COLOR RGBA_32

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
  void project_to(FBuff<Color_t>* project_to);
  void move(SCR_UINT dx, SCR_UINT dy);
  void free();
  void premultiply();

  // simple draw methods
  void DrawRect(Rect<SCR_UINT>& rect, Color_t& color);
  void DrawBounds(Rect<SCR_UINT>& rect, Color_t& color, short thickness);
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
  pxls = ALLOC_AR(Color_t, height * (__int64)width);
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
  DEALLOC(pxls);
  pxls = ALLOC_AR(Color_t, height * (__int64)width);
  size.assign(width, height);
}

template <typename Color_t>
void FBuff<Color_t>::free() {
  if (pxls) {
    DELETE_DBG_AR(pxls);
  }
  pxls = nullptr;
  size.assign(0, 0);
}

#define LOOP_PROJECT_TO(idxfrom)                                                            \
  for (SCR_UINT i = 0; i < lastpxlx; i++) {                                                 \
    for (SCR_UINT j = 0; j < lastpxly; j++) {                                               \
      SCR_UINT to = project_to->size.x * (projectrect.pos.y + j) + (projectrect.pos.x + i); \
      SCR_UINT from = idxfrom;                                                              \
      project_to->pxls[to] = pxls[from];                                                    \
    }                                                                                       \
  }

template <typename Color_t>
void FBuff<Color_t>::project_to(FBuff<Color_t>* project_to) {

  Rect<SCR_UINT> rect(pos, size);
  Rect<SCR_UINT> prntrect(vec2<SCR_UINT>(), project_to->size);

  Rect<SCR_UINT> projectrect;
  rect.intersection(prntrect, projectrect);

  SCR_UINT lastpxlx = projectrect.size.x;
  SCR_UINT lastpxly = projectrect.size.y;

  vec2<SCR_UINT> offset(rect.size - projectrect.size);

  vec2<bool> mns(rect.pos.x < 0, rect.pos.y < 0);

  if (mns.x && mns.y) {
    LOOP_PROJECT_TO(size.x * (j + offset.y) + (i + offset.x));
  } else if (!mns.x && !mns.y) {
    LOOP_PROJECT_TO(size.x * j + i);
  } else if (!mns.x && mns.y) {
    LOOP_PROJECT_TO(size.x * (j + offset.y) + i);
  } else {
    LOOP_PROJECT_TO(size.x * j + (i + offset.x));
  }
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

  Rect<SCR_UINT> myrect(vec2<SCR_UINT>(), size);
  
  Rect<SCR_UINT> projectrect;
  myrect.intersection(rect, projectrect);

  SCR_UINT lastpxlx = projectrect.pos.x + projectrect.size.x;
  SCR_UINT lastpxly = projectrect.pos.y + projectrect.size.y;

  for (SCR_UINT i = projectrect.pos.x; i < lastpxlx; i++) {
    for (SCR_UINT j = projectrect.pos.y; j < lastpxly; j++) {
      set(i, j, &color);
    }
  }
}

template <typename Color_t>
void FBuff<Color_t>::DrawBounds(Rect<SCR_UINT>& rect, Color_t& color, short thickness) {

  Rect<SCR_UINT> myrect(vec2<SCR_UINT>(), size);

  Rect<SCR_UINT> projectrect;
  myrect.intersection(rect, projectrect);

  SCR_UINT lastpxlx = projectrect.pos.x + projectrect.size.x - 1;
  SCR_UINT lastpxly = projectrect.pos.y + projectrect.size.y - 1;

  short th1 = 0;
  short th2 = thickness;

  while (th1 < th2) {
    for (SCR_UINT i = projectrect.pos.x; i < lastpxlx; i++) {
      set(i, projectrect.pos.y + th1, &color);
      set(i, lastpxly - th1, &color);
    }

    for (SCR_UINT j = projectrect.pos.y; j < lastpxly; j++) {
      set(projectrect.pos.x + th1, j, &color);
      set(lastpxlx - th1, j, &color);
    }

    th1++;
  }
}

template <typename Color_t>
Color_t* FBuff<Color_t>::get(SCR_UINT x, SCR_UINT y) {

  if (!local_hrchy.prnt) {
    return pxls + (__int64)size.x * y + x;
  }

  return local_hrchy.prnt->pxls + *root_width * ((__int64)y + pos.y) + x + pos.x;
}

template <typename Color_t>
void FBuff<Color_t>::coppy(FBuff<RGBAf>* fbuff) {
  resize(fbuff->size.x, fbuff->size.y);
  unsigned char r, g, b;
  SCR_UINT len = size.x * size.y;
  for (SCR_UINT i = 0; i < len; i++) {
    r = unsigned char((fbuff->pxls + i)->R * 255);
    g = unsigned char((fbuff->pxls + i)->G * 255);
    b = unsigned char((fbuff->pxls + i)->B * 255);

    *(pxls + i) = 0xff000000 | ((RGBA_32)r << 16) | ((RGBA_32)g << 8) | (RGBA_32)b;
  }
}

template <typename Color_t>
void FBuff<Color_t>::cast(FBuff& out, Rect<SCR_UINT>& rect) {

  assert(size >= rect->size && rect->pos >= pos);

  local_hrchy.childs.add(&out);

  out.local_hrchy.prnt = this;

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
  DEALLOC(pxls);
  local_hrchy.leave();
}
