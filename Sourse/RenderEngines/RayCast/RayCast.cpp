#include "public/RayCast.h"

#include "Ray.h"

using namespace RayCast;


struct CycleData;
void RenderPass(RenderSettings* settings, Ray ray, Color4& color);
void WritePassToBuff(FBuff* Buff, int i, int j, CycleData& CData,
                     Color4& color);
void InitCycle(CycleData& CData, RenderSettings* settings);
inline void UpdYPass(CycleData& CData, int& i);
inline void UpdXPass(CycleData& CData, int& i);

struct CycleData {
  int x_passes = 0, y_passes = 0;
  Vec3f step_x, step_y;
  Ray CamRay;
  int pxl_size = 0;
  Camera* CamAtrb = nullptr;
};

void RayCast::RenderToBuff(RenderSettings* Settings, FBuff* Buff) {
  CycleData CData;
  InitCycle(CData, Settings);

  for (int j = 0; j < CData.y_passes; UpdYPass(CData, j)) {
    for (int i = 0; i < CData.x_passes; UpdXPass(CData, i)) {
      Color4 Color;
      RenderPass(Settings, CData.CamRay, Color);
      WritePassToBuff(Buff, i, j, CData, Color);
    }
  }
}

void RenderPass(RenderSettings* settings, Ray ray, Color4& color) {
  ray.Dir.Normalize();

  ray.Cast(settings->getObjList(), FLT_MAX);
  if (ray.HitData.Hit) {
    color.B = 1.f;
  } else {
    color.R = 1.f;
  }
}

void WritePassToBuff(FBuff* Buff, int i, int j, CycleData& CData,
                     Color4& color) {
  if (CData.pxl_size == 1) {
    Buff->set(i, j, &color);
    return;
  }

  int pos_x = i * CData.pxl_size - (CData.pxl_size / 2);
  int pos_y = j * CData.pxl_size - (CData.pxl_size / 2);
  CLAMP(pos_x, 0, INT_MAX);
  CLAMP(pos_y, 0, INT_MAX);
  Buff->DrawRect(pos_x, pos_y, color, CData.pxl_size, CData.pxl_size);
}

void InitCycle(CycleData& CData, RenderSettings* settings) {
  Object* cam = settings->getCamera();
  Camera* CamAtrb = cam->GetCameraComponent();
  Mat3f* Rotation = &cam->TransformMat;

  float cam_y =
      (float)sqrt(float(CamAtrb->Height.get()) / CamAtrb->Width.get());
  float cam_x = 1 / cam_y;

  int pxl_size = (int)floor(1.f / settings->Resolution.get());

  if (pxl_size != 1 && pxl_size & 1) {
    pxl_size++;
  }

  CData.y_passes = (int)floor(CamAtrb->Height.get() / pxl_size) - 1;
  CData.x_passes = (int)floor(CamAtrb->Width.get() / pxl_size) - 1;

  CData.step_x = Rotation->I * (cam_x / CData.x_passes);
  CData.step_y = Rotation->J * (-cam_y / CData.y_passes);

  cam->getWorldLocation(CData.CamRay.Pos);

  CData.CamRay.Dir -= Rotation->I * (1.f / 2 * cam_x);
  CData.CamRay.Dir += Rotation->J * (1.f / 2 * cam_y);
  CData.CamRay.Dir -= Rotation->K * CamAtrb->Lens.get();
  CData.CamRay.Dir += (CData.step_x / 2) + (CData.step_y / 2);
  CData.CamRay.Dir.Normalize();

  CData.CamAtrb = CamAtrb;
  CData.pxl_size = pxl_size;
}

inline void UpdYPass(CycleData& CData, int& i) {
  i++;
  CData.CamRay.Dir -= CData.step_x * (float)(CData.x_passes + 1);
  CData.CamRay.Dir += CData.step_y;
}

inline void UpdXPass(CycleData& CData, int& i) {
  i++;
  CData.CamRay.Dir += CData.step_x;
}

RenderSettings::RenderSettings() {
  ObjList = nullptr;
  Camera = nullptr;
  Resolution.assign(0.5f, 0.1f, 1.f);
}

RenderSettings::~RenderSettings() {}

void RenderSettings::setObjList(List<Object>* list) { ObjList = list; }

void RenderSettings::setCamera(Object* camera) { Camera = camera; }

Object* RayCast::RenderSettings::getCamera() { return Camera; }

List<Object>* RayCast::RenderSettings::getObjList() { return ObjList; }