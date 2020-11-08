#include "public/RayCast.h"
#include "Ray.h"

#define FOREACH_PASS(iter, passes_num) for (int iter = 0; iter < passes_num; iter++)
using namespace RayCast;

struct CycleData {
	int x_passes = 0, y_passes = 0;
	Vec3f step_x, step_y;
	Vec3f EntryDir, CamPos;

	int pxl_size = 0;
	Camera* CamAtrb = nullptr;
};

void CalcColorFromCamRay(Color4& color, Ray& ray, RenderSettings* settings);
void WritePassToBuff(FBuff* Buff, int i, int j, CycleData& CData, Color4& color);
void InitCycle(CycleData& CData, RenderSettings* settings);

RenderSettings::RenderSettings() {
	ObjList = nullptr;
	Camera = nullptr;
	Resolution.assign(0.f, 0.5f, 1.f);
}

RenderSettings::~RenderSettings() {
}

void RenderSettings::setObjList(List<Object>* list) {
	ObjList = list;
}

void RenderSettings::setCamera(Object* camera) {
	Camera = camera;
}

Object* RayCast::RenderSettings::getCamera()
{
	return Camera;
}

List<Object>* RayCast::RenderSettings::getObjList()
{
	return ObjList;
}


void RayCast::RenderToBuff(RenderSettings* settings, FBuff* Buff) {

	CycleData CData;
	InitCycle(CData, settings);

	Ray CamRay = Ray(CData.CamPos, CData.EntryDir);

	Color4 color;

	FOREACH_PASS(j, CData.y_passes) {
		FOREACH_PASS(i, CData.x_passes) {

			CalcColorFromCamRay(color, CamRay, settings);
			WritePassToBuff(Buff, i, j, CData, color);

			CamRay.Dir += CData.step_x;
		}
		CamRay.Dir += CData.step_y;
	}
}

void InitCycle(CycleData& CData, RenderSettings* settings) {

	Object* cam = settings->getCamera();
	Camera* CamAtrb = cam->GetCameraComponent();
	Mat3f* Rotation = &cam->TransformMat;

	float cam_y = (float)sqrt(float(CamAtrb->Height.get()) / CamAtrb->Width.get());
	float cam_x = 1 / cam_y;

	int pxl_size = (int)floor(1.f / settings->Resolution.get());
	CData.y_passes = (int)floor(CamAtrb->Height.get() / pxl_size);
	CData.x_passes = (int)floor(CamAtrb->Width.get() / pxl_size);

	CData.step_x = Rotation->I * (cam_x / CData.x_passes);
	CData.step_y = Rotation->J * (-cam_y / CData.y_passes);

	cam->getWorldLocation(CData.CamPos);

	CData.EntryDir = CData.CamPos;
	CData.EntryDir += Rotation->I * (1 / 2 * cam_x) + Rotation->J * (1 / 2 * cam_y) - Rotation->K;
	CData.EntryDir += (CData.step_x / 2) + (CData.step_y / 2);

	CData.CamAtrb = CamAtrb;
	CData.pxl_size = pxl_size;
}

void CalcColorFromCamRay(Color4& color, Ray& ray, RenderSettings* settings) {
	ray.Cast(settings->getObjList(), FLT_MAX);
	if (ray.HitData.Hit) {
		color.G = color.B = color.R = 255;
	}
}

void WritePassToBuff(FBuff* Buff, int i, int j, CycleData& CData, Color4& color) {
	int pos_x = (CData.x_passes - i) * CData.pxl_size - CData.pxl_size/2;
	int pos_y = j* CData.pxl_size - CData.pxl_size / 2;
	Buff->DrawRect(pos_x, pos_y, color, CData.pxl_size, CData.pxl_size);
}