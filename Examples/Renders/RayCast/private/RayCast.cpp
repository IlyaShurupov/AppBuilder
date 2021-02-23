#include "../RayCast.h"
#include "Object/Object.h"
#include "Geometry/Ray.h"
#include "BitMap/BitMap.h"

struct CycleData;
void RenderPass(RenderSettings* settings, Ray ray, COLOR& color);
void WritePassToBuff(BitMap<RGBA_32>* Buff, SCR_INT i, SCR_INT j, CycleData& CData, COLOR& color);
void InitCycle(CycleData& CData, RenderSettings* settings, BitMap<RGBA_32>* Buff);
inline void UpdYPass(CycleData& CData, SCR_INT& i);
inline void UpdXPass(CycleData& CData, SCR_INT& i);

struct CycleData {
  SCR_INT x_passes = 0, y_passes = 0;
  Vec3f step_x, step_y;
  Ray CamRay;
  SCR_INT pxl_size = 0;
  Camera* CamAtrb = nullptr;
};

void RenderToBuff(RenderSettings* Settings, BitMap<RGBA_32>* Buff) {

  CycleData CData;
  InitCycle(CData, Settings, Buff);

  for (SCR_INT j = 0; j < CData.y_passes; UpdYPass(CData, j)) {
    for (SCR_INT i = 0; i < CData.x_passes; UpdXPass(CData, i)) {
      COLOR Color;
      RenderPass(Settings, CData.CamRay, Color);
      WritePassToBuff(Buff, i, j, CData, Color);
    }
  }
}

void RenderPass(RenderSettings* settings, Ray ray, COLOR& color) {
  ray.Dir.Normalize();

  ray.Cast(settings->getObjList(), FLT_MAX);
  if (ray.HitData.Hit) {
    color = 0xffff0000;
  } else {
    color = 0xffffff00;
  }
}

void WritePassToBuff(BitMap<RGBA_32>* Buff, SCR_INT i, SCR_INT j, CycleData& CData, COLOR& color) {
  if (CData.pxl_size == 1) {
    Buff->Set(i, j, &color);
    return;
  }

  SCR_INT pos_x = i * CData.pxl_size - (CData.pxl_size / 2);
  SCR_INT pos_y = j * CData.pxl_size - (CData.pxl_size / 2);
  CLAMP(pos_x, (SCR_INT)0, SCR_UINT_MAX);
  CLAMP(pos_y, (SCR_INT)0, SCR_UINT_MAX);


   Buff->DrawRect(Rect<SCR_INT>(pos_x, pos_y, CData.pxl_size, CData.pxl_size), color);
}

void InitCycle(CycleData& CData, RenderSettings* settings, BitMap<RGBA_32>* Buff) {
  Object* cam = settings->getCamera();
  Camera* CamAtrb = cam->GetCameraComponent();
  Mat3f* Rotation = &cam->TransformMat;

  float cam_y = (float)sqrt(float(Buff->size.y) / Buff->size.x);
  float cam_x = 1 / cam_y;

  SCR_INT pxl_size = (SCR_INT)floor(1.f / settings->Resolution.get());

  if (pxl_size != 1 && pxl_size & 1) {
    pxl_size++;
  }

  CData.y_passes = (SCR_INT)floor(Buff->size.y / pxl_size);
  CData.x_passes = (SCR_INT)floor(Buff->size.x / pxl_size);

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

inline void UpdYPass(CycleData& CData, SCR_INT& i) {
  i++;
  CData.CamRay.Dir -= CData.step_x * (float)(CData.x_passes + 1);
  CData.CamRay.Dir += CData.step_y;
}

inline void UpdXPass(CycleData& CData, SCR_INT& i) {
  i++;
  CData.CamRay.Dir += CData.step_x;
}
