#include "Ray.h"

Ray::Ray() {
}

Ray::Ray(Vec3f& Dir, Vec3f& Pos) {
	this->Dir = Dir;
	this->Pos = Pos;
	HitData.Hit = false;
}

Ray::~Ray() {
}

void WorldTransform(Trig* trig, Vec3f* pos, Mat3f* mat) {

	mat->Transform(trig->V0);
	mat->Transform(trig->V1);
	mat->Transform(trig->V2);

	trig->V0 + *pos;
	trig->V1 + *pos;
	trig->V2 + *pos;
}

void Ray::Cast(List<Object>* objects, float ray_length)
{
	HitData.Hit = false;

	FOREACH(objects, Object, obj) {

		StaticMesh* mesh = obj->GetStaticMeshComponent();
		if (!mesh) {
			continue;
		}

		List<Trig>* trigs = &mesh->Trigs;
		FOREACH(trigs, Trig, trig) {

			WorldTransform(trig.Node()->data, &obj->Pos, &obj->TransformMat);

			Vec3f HitPos;
			if (!trig->RayHit(*this, HitPos)) {
				continue;
			}

			float dist = (float)(HitPos - Pos).Length();
			if (dist < ray_length) {
				
				HitData.Hit = true;
				HitData.Obj = obj.Node()->data;
				HitData.trig = trig.Node()->data;
				HitData.HitPos = HitPos;

				ray_length = dist;
			}
		}
	}
}

