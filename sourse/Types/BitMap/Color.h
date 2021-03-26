#pragma once

#define RGBA_32 int
#define COLOR RGBA_32

#include "Macros.h"

class Color {
  public:
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 1.f;
    
    Color() {
    }

    Color(float r, float g, float b, float a) {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = a;
    }

    uint4 to_argb32() {
      uint4 color = (uint1)(a * 255);
      color <<= 8;
      color |= (uint1)(r * 255);
      color <<= 8;
      color |= (uint1)(g * 255);
      color <<= 8;
      color |= (uint1)(b * 255);

      return color;
    }

    void Assign(float p_r, float p_g, float p_b, float p_a) {
      r = p_r;
      g = p_g;
      b = p_b;
      a = p_a;
    }

    ~Color() {
    }

};