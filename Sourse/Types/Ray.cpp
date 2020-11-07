#include "public/Ray.h"

Ray::Ray() {
}

Ray::~Ray() {
}

void Ray::Cast(List<Object>* objects, float ray_length)
{
	FOREACH_NODE(Object, objects, ob_node) {

		StaticMesh* mesh = ob_node->Data->GetStaticMeshComponent();
		if (!mesh) {
			continue;
		}

		List<Trig>* trigs = &mesh->Trigs;
		FOREACH_NODE(Trig, trigs, trig_node) {

			Vec3f HitPos;
			if (!trig_node->Data->RayHit(this, HitPos)) {
				continue;
			}

			float dist = (HitPos - Pos).Length();
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

