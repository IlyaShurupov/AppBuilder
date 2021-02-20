
#pragma once

#include "Geometry/Rect.h"
#include "Memory/Allocators.h"
#include "Color.h"

template <typename BMType>
struct BitMap {

  vec2<int> size;
  BMType* pxlbuff = nullptr;

  BitMap() {}
  template <typename SizeType>
  BitMap(SizeType width, SizeType height) {
    size.assign(width, height);
    pxlbuff = ALLOC_AR(BMType, (int)height * (__int64)width);
  }
  ~BitMap() {
    DEALLOC(pxlbuff);
  }

  BMType* Get(int x, int y) { return pxlbuff + (__int64)size.x * y + x; }
  void Set(int x, int y, BMType* value) { *Get(x, y) = *value; }

  void resize(int width, int height) {
    DEALLOC(pxlbuff);
    pxlbuff = ALLOC_AR(BMType, height * (__int64)width);
    size.assign(width, height);
  }

  void Project(BitMap<BMType>* buff, vec2<int>& pos) {
    
    Rect<int> unclamped_fromrect(pos, buff->size);
    Rect<int> fromrect;
    Rect<int> torect(vec2<int>(), size);
    
    torect.intersection(unclamped_fromrect, fromrect);

    vec2<int> clampsize(unclamped_fromrect.size - fromrect.size);

    vec2<bool> mns(unclamped_fromrect.pos.x < 0, unclamped_fromrect.pos.y < 0);

    if (mns.x && mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          pxlbuff[to] = buff->pxlbuff[buff->size.x * (j + clampsize.y) + (i + clampsize.x)];
        }
      }
    } else if (!mns.x && !mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          pxlbuff[to] = buff->pxlbuff[buff->size.x * j + i];
        }
      }
    } else if (!mns.x && mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          pxlbuff[to] = buff->pxlbuff[buff->size.x * (j + clampsize.y) + i];
        }
      }
    } else {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          pxlbuff[to] = buff->pxlbuff[buff->size.x * j + (i + clampsize.x)];
        }
      }
    }
  }

  // draw methods
  void DrawRect(Rect<int>& rect, BMType& value) {
    Rect<int> myrect(vec2<int>(), size);
    Rect<int> projectrect;
    myrect.intersection(rect, projectrect);
    int lastpxlx = projectrect.pos.x + projectrect.size.x;
    int lastpxly = projectrect.pos.y + projectrect.size.y;
    for (int i = projectrect.pos.x; i < lastpxlx; i++) {
      for (int j = projectrect.pos.y; j < lastpxly; j++) {
        Set(i, j, &value);
      }
    }
  }

  void DrawBounds(Rect<int>& rect, BMType& value, short thickness) {
    Rect<int> myrect(vec2<int>(), size);
    Rect<int> projectrect;
    myrect.intersection(rect, projectrect);
    int lastpxlx = projectrect.pos.x + projectrect.size.x - 1;
    int lastpxly = projectrect.pos.y + projectrect.size.y - 1;
    short th1 = 0;
    short th2 = thickness;
    while (th1 < th2) {
      if (thickness < projectrect.size.y) {
        for (int i = projectrect.pos.x; i < lastpxlx; i++) {
          Set(i, projectrect.pos.y + th1, &value);
          Set(i, lastpxly - th1, &value);
        }
      }
      if (thickness < projectrect.size.x) {
        for (int j = projectrect.pos.y; j < lastpxly; j++) {
          Set(projectrect.pos.x + th1, j, &value);
          Set(lastpxlx - th1, j, &value);
        }
      }
      th1++;
    }
  }

  void Assign(BMType* value) {
    long int len = size.x * size.y;
    for (int i = 0; i < len; i++) {
      pxlbuff[i] = *value;
    }
  }
};