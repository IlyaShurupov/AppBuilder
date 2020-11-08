#include "public/Ray.h"

Ray::Ray() {
}

Ray::Ray(Vec3f& Dir, Vec3f& Pos) {
	this->Dir = Dir;
	this->Pos = Pos;
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
	FOREACH_NODE(Object, objects, ob_node) {
		Object* obj = ob_node->Data;

		StaticMesh* mesh = obj->GetStaticMeshComponent();
		if (!mesh) {
			continue;
		}

		List<Trig>* trigs = &mesh->Trigs;
		FOREACH_NODE(Trig, trigs, trig_node) {

			Trig trig = Trig(*trig_node->Data);

			WorldTransform(&trig, &obj->Pos, &obj->TransformMat);

			Vec3f HitPos;
			if (!trig.RayHit(this, HitPos)) {
				continue;
			}

			float dist = (float)(HitPos - Pos).Length();
			if (dist < ray_length) {
				
				HitData.Hit = true;
				HitData.Obj = ob_node->Data;
				HitData.trig = trig_node->Data;
				HitData.HitPos = HitPos;

				ray_length = dist;
			}
		}
	}
}

