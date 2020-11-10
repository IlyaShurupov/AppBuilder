#include "public/Print.h"

#include <windows.h>

#define COUT std::cout <<
#define TAB "	" <<
#define NL << "\n"
#define SPC << " " <<
#define VEC(vec) vec.x SPC vec.y SPC vec.z
#define TRG(trig) \
  "v0:" << VEC(trig.V0) << "v1:" << VEC(trig.V1) << "v2:" << VEC(trig.V2)
#define ENTR COUT "\n";
#define CLASS_NAME(Type) << typeid(Type).name()
#define PROP(prop) prop.get()

void print(Vec3f& vec) { ENTR COUT "Vec3f: " << VEC(vec) NL; }

void print(Mat3f& mat) {
  ENTR COUT "Mat3f: \n";
  COUT TAB VEC(mat.I) NL;
  COUT TAB VEC(mat.J) NL;
  COUT TAB VEC(mat.K) NL;
}

void print(PropertyFloat& prp) {
  ENTR COUT "Float property: " NL;
  COUT TAB "Val:" << prp.get() NL;
  COUT TAB "Min:" << prp.min NL;
  COUT TAB "Max:" << prp.max NL;
}

void print(PropertyInt& prp) {
  ENTR COUT "Int property: " NL;
  COUT TAB "Val:" << prp.get() NL;
  COUT TAB "Min:" << prp.min NL;
  COUT TAB "Max:" << prp.max NL;
}

void print(RayHitData& HD) {
  ENTR COUT "RayHitData: " NL;
  COUT TAB "Bool Hit: " << HD.Hit NL;
  COUT TAB "Hit Pos: " << VEC(HD.HitPos) NL;
  COUT TAB "Trig: " << TRG((*HD.trig)) NL;
  COUT TAB "Hit OBJ Pos: " << VEC(HD.Obj->Pos) NL;
}

void print(Ray& ray) {
  ENTR COUT VEC(ray.Dir);
  COUT TAB "Pos: " << VEC(ray.Pos) NL;
  COUT TAB "Dir: " << VEC(ray.Dir) NL;
}

void print(Camera& cam) {
  ENTR COUT "Camera: " NL;
  COUT TAB "Height" << PROP(cam.Height) NL;
  COUT TAB "Width" << PROP(cam.Width) NL;
  COUT TAB "Lens" << PROP(cam.Lens) NL;
}

void print(FBuff& buff, bool PrintText) {
  if (PrintText) {
    HWND myconsole = GetConsoleWindow();
    HDC mydc = GetDC(myconsole);

    for (SCR_UINT j = 0; j < buff.height; j++) {
      for (SCR_UINT i = 0; i < buff.width; i++) {
        Color4* color = buff.get(i, j);
        COLORREF COLOR = RGB(color->R * 255, color->G * 255, color->B * 255);
        // COLORREF COLOR = RGB(0, 255, 12);
        SetPixel(mydc, (int)i + 30, (int)j + 30, COLOR);
      }
    }
    ReleaseDC(myconsole, mydc);
    std::cin.ignore();
  } else {
    ENTR COUT "Frame Buffer: " NL;
    COUT TAB "Height: " << buff.height NL;
    COUT TAB "Width: " << buff.width NL;
  }
}